#include "zwrecordthread.h"
#include <QDebug>
#include "zwlog.h"
#include <QFileInfo>

//g_collectFrameCnt_zw等于g_encodeFrameCnt_zw证明编解码帧数一致
int g_collectFrameCnt_zw = 0;	//采集帧数
int g_encodeFrameCnt_zw = 0;	//编码帧数
using namespace std;
using namespace  ZW_LOG;

#define RETBREAK(v,error) if(v < 0){ZW_LOG_INFO(QString("ret=%1").arg(v)+error);ret=v;break;}

ZWRecordThread::ZWRecordThread(QObject *parent)
    :QThread(parent)
    , m_fps(30)
    , m_vIndex(-1)
    , m_vOutIndex(-1)
    , m_vFmtCtx(nullptr),
      m_oFmtCtx(nullptr)
    , m_vDecodeCtx(nullptr)
    , m_vEncodeCtx(nullptr)
    , m_dict(nullptr)
    , m_vFifoBuf(nullptr)
    , m_swsCtx(nullptr)
    , m_state(RecordState::NotStarted)
{
    m_width = 1280;
    m_height = 720;
    m_fps = 30;
    m_filePath = "/Users/miaozw/Documents/TEMP/test-out.flv";
}

ZWRecordThread::~ZWRecordThread()
{

}

void ZWRecordThread::Start()
{
    if (m_state == RecordState::NotStarted)
    {
        ZW_LOG_INFO("start record");
        m_state = RecordState::Started;
        std::thread recordThread(&ZWRecordThread::ScreenRecordThreadProc, this);
        recordThread.detach();
    }
    else if (m_state == RecordState::Paused)
    {
        ZW_LOG_INFO("continue record");
        m_state = RecordState::Started;
        m_cvNotPause.notify_one();
    }
}

void ZWRecordThread::Pause()
{
    ZW_LOG_INFO("pause record");
    m_state = RecordState::Paused;
}

void ZWRecordThread::Stop()
{
    qDebug() << "stop record";
    if (m_state == RecordState::Paused)
        m_cvNotPause.notify_one();
    m_state = RecordState::Stopped;
}

void ZWRecordThread::ScreenRecordThreadProc()
{
    int ret = -1;
    //减小原子变量粒度
     bool done = false;
     int64_t vCurPts = 0;
     int vFrameIndex = 0;

    av_register_all();
    avdevice_register_all();
    avcodec_register_all();
    if(openVideo() < 0)
    {
        return;
    }
    if(OpenOutput() < 0)
    {
        return;
    }
    if(InitBuffer() < 0)
    {
        return;
    }

    //启动视频数据采集线程
    std::thread screenRecord(&ZWRecordThread::ScreenAcquireThreadProc, this);
    screenRecord.detach();

    while (1)
    {
        if (m_state == RecordState::Stopped && !done)
            done = true;
        if (done)
        {
            lock_guard<mutex> lk(m_mtx);
            if (av_fifo_size(m_vFifoBuf) < m_vOutFrameSize)
                break;
        }
        {
            std::unique_lock<mutex> lk(m_mtx);
            m_cvNotEmpty.wait(lk, [this] {return av_fifo_size(m_vFifoBuf) >= m_vOutFrameSize; });
        }
        av_fifo_generic_read(m_vFifoBuf, m_vOutFrameBuf, m_vOutFrameSize, NULL);
        m_cvNotFull.notify_one();

        //设置视频帧参数
        //m_vOutFrame->pts = vFrameIndex * ((m_oFmtCtx->streams[m_vOutIndex]->time_base.den / m_oFmtCtx->streams[m_vOutIndex]->time_base.num) / m_fps);
        m_vOutFrame->pts = vFrameIndex;
        ++vFrameIndex;
        m_vOutFrame->format = m_vEncodeCtx->pix_fmt;
        m_vOutFrame->width = m_width;
        m_vOutFrame->height = m_height;
        AVPacket pkt = { 0 };
        av_init_packet(&pkt);

        ret = avcodec_send_frame(m_vEncodeCtx, m_vOutFrame);
        if (ret != 0)
        {
            ZW_LOG_WARNING("video avcodec_send_frame failed "+ZW_LOG_P(ret));
            av_packet_unref(&pkt);
            continue;
        }
        ret = avcodec_receive_packet(m_vEncodeCtx, &pkt);
        if (ret != 0)
        {
            av_packet_unref(&pkt);
            if (ret == AVERROR(EAGAIN))
            {
                ZW_LOG_WARNING("EAGAIN avcodec_receive_packet"+ZW_LOG_P(ret));
                continue;
            }
            ZW_LOG_WARNING("video avcodec_receive_packet failed "+ZW_LOG_P(ret));
            return;
        }
        pkt.stream_index = m_vOutIndex;
        av_packet_rescale_ts(&pkt, m_vEncodeCtx->time_base, m_oFmtCtx->streams[m_vOutIndex]->time_base);

        ret = av_interleaved_write_frame(m_oFmtCtx, &pkt);
        if (ret == 0)
        {
//            ZW_LOG_INFO("Write video packet id: "+ZW_LOG_P(g_encodeFrameCnt_zw));
            g_encodeFrameCnt_zw++;
        }
        else
            ZW_LOG_WARNING("video av_interleaved_write_frame failed "+ZW_LOG_P(ret));
        av_free_packet(&pkt);
    }
    FlushEncoder();
    av_write_trailer(m_oFmtCtx);
    Release();
    ZW_LOG_INFO("parent thread exit");
}

void ZWRecordThread::ScreenAcquireThreadProc()
{
    int ret = -1;
    AVPacket pkt = { 0 };
    av_init_packet(&pkt);
    int y_size = m_width * m_height;
    AVFrame	*oldFrame = av_frame_alloc();
    AVFrame *newFrame = av_frame_alloc();

    int newFrameBufSize = av_image_get_buffer_size(m_vEncodeCtx->pix_fmt, m_width, m_height, 1);
    uint8_t *newFrameBuf = (uint8_t*)av_malloc(newFrameBufSize);
    av_image_fill_arrays(newFrame->data, newFrame->linesize, newFrameBuf,
                         m_vEncodeCtx->pix_fmt, m_width, m_height, 1);

    while (m_state != RecordState::Stopped)
    {
        if (m_state == RecordState::Paused)
        {
            unique_lock<mutex> lk(m_mtxPause);
            //在第二种情况下（即设置了 Predicate），
            //只有当 pred 条件为false 时调用 wait() 才会阻塞当前线程
            //，并且在收到其他线程的通知后只有当 pred 为 true 时才会被解除阻塞。其内部代码
            //while (!pred()) wait(lck);
            m_cvNotPause.wait(lk, [this] {
                bool temp = m_state != RecordState::Paused;
                ZW_LOG_INFO(ZW_LOG_P2(m_state,temp));
                return temp;
//                return false;
            });
            ZW_LOG_INFO("ScreenAcquireThreadProc"+ZW_LOG_P(m_state));
        }
        if (av_read_frame(m_vFmtCtx, &pkt) < 0)
        {
            ZW_LOG_WARNING("video av_read_frame < 0");
            continue;
        }
        if (pkt.stream_index != m_vIndex)
        {
            ZW_LOG_WARNING("not a video packet from video input");
            av_packet_unref(&pkt);
            continue;
        }
        ret = avcodec_send_packet(m_vDecodeCtx, &pkt);
        if (ret != 0)
        {
            ZW_LOG_WARNING("avcodec_send_packet failed"+ZW_LOG_P(ret));
            av_packet_unref(&pkt);
            continue;
        }
        ret = avcodec_receive_frame(m_vDecodeCtx, oldFrame);
        if (ret != 0)
        {
            ZW_LOG_WARNING("avcodec_receive_frame failed"+ZW_LOG_P(ret));
            av_packet_unref(&pkt);
            continue;
        }
        ++g_collectFrameCnt_zw;

        //srcSliceH到底是输入高度还是输出高度
        sws_scale(m_swsCtx, (const uint8_t* const*)oldFrame->data, oldFrame->linesize, 0,
                  m_vEncodeCtx->height, newFrame->data, newFrame->linesize);

        {
            unique_lock<mutex> lk(m_mtx);
            m_cvNotFull.wait(lk, [this] { return av_fifo_space(m_vFifoBuf) >= m_vOutFrameSize; });
        }
        av_fifo_generic_write(m_vFifoBuf, newFrame->data[0], y_size, NULL);
        av_fifo_generic_write(m_vFifoBuf, newFrame->data[1], y_size / 4, NULL);
        av_fifo_generic_write(m_vFifoBuf, newFrame->data[2], y_size / 4, NULL);
        m_cvNotEmpty.notify_one();

        av_packet_unref(&pkt);
    }
    FlushDecoder();

    av_free(newFrameBuf);
    av_frame_free(&oldFrame);
    av_frame_free(&newFrame);
    qDebug() << "screen record thread exit";
}

int ZWRecordThread::openVideo()
{
    int ret = -1;
    do{
        AVInputFormat *ifmt = av_find_input_format("avfoundation");
        if(ifmt == NULL)
        {
            ZW_LOG_INFO("av_find_input_format avfoundation is null");
            break;
        }
        AVDictionary *options = nullptr;
        //https://blog.csdn.net/MoSee/article/details/78004234
        //mac 摄像头需要设置参数
        av_dict_set(&options, "framerate", "30", 0);
//        av_dict_set(&options, "video_size", "1280x720", 0);
        av_dict_set(&options, "pixel_format", "uyvy422", 0);

#ifdef Q_OS_MAC
        ret = avformat_open_input(&m_vFmtCtx,"1",ifmt,&options);
#else
        ret = avformat_open_input(&m_vFmtCtx,"desktop",ifmt,&options);
#endif
        RETBREAK(ret,"avformat_open_input is error");
        ret = avformat_find_stream_info(m_vFmtCtx,NULL);
        RETBREAK(ret,"avformat_find_stream_info is error");
        AVCodec *decoder = nullptr;
        for (int i = 0; i < m_vFmtCtx->nb_streams; ++i)
        {
            AVStream *stream = m_vFmtCtx->streams[i];
            if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            {
                decoder = avcodec_find_decoder(stream->codecpar->codec_id);
                if (decoder == nullptr)
                {
                    ZW_LOG_WARNING("avcodec_find_decoder failed");
                    ret = -1;
                    break;
                }
                //从视频流中拷贝参数到codecCtx
                m_vDecodeCtx = avcodec_alloc_context3(decoder);
                ret = avcodec_parameters_to_context(m_vDecodeCtx, stream->codecpar);
                RETBREAK(ret,"Video avcodec_parameters_to_context failed");
                m_vIndex = i;
                break;
            }
        }
        ret = avcodec_open2(m_vDecodeCtx, decoder, &m_dict);
        RETBREAK(ret,"avcodec_open2t failed");
//#define SWS_FAST_BILINEAR     1
//#define SWS_BILINEAR          2
//#define SWS_BICUBIC           4
//#define SWS_X                 8
//#define SWS_POINT          0x10
//#define SWS_AREA           0x20
//#define SWS_BICUBLIN       0x40
//#define SWS_GAUSS          0x80
//#define SWS_SINC          0x100
//#define SWS_LANCZOS       0x200
//#define SWS_SPLINE        0x400
        m_swsCtx = sws_getContext(m_vDecodeCtx->width, m_vDecodeCtx->height, m_vDecodeCtx->pix_fmt,
            m_width, m_height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);
        ret = 0;
    }while(0);

    return ret;
}

int ZWRecordThread::OpenOutput()
{
    int ret = -1;
    do{
        std::string outFilePath = m_filePath.toStdString();
        ret = avformat_alloc_output_context2(&m_oFmtCtx,NULL,NULL,outFilePath.c_str());
        RETBREAK(ret,"avformat_alloc_output_context2 failed");
        if(m_vFmtCtx->streams[m_vIndex]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            AVStream *vStream = nullptr;
            vStream = avformat_new_stream(m_oFmtCtx,nullptr);
            if(vStream == NULL)
            {
                RETBREAK(-1,"avformat_new_stream failed");
            }
            //AVFormatContext第一个创建的流索引是0，第二个创建的流索引是1
            m_vOutIndex = vStream->index;
            vStream->time_base = AVRational{ 1, m_fps };
            m_vEncodeCtx = avcodec_alloc_context3(NULL);
            if (nullptr == m_vEncodeCtx)
            {
                RETBREAK(-1,"avcodec_alloc_context3 failed");
            }
            //设置编码参数
            SetEncoderParm();
            //查找视频编码器
            AVCodec *encoder;
            encoder = avcodec_find_encoder(m_vEncodeCtx->codec_id);
            if (!encoder)
            {
                QString error(QString("Can not find the encoder, id: %1").arg(m_vEncodeCtx->codec_id));
                RETBREAK(-1,error);
            }
            m_vEncodeCtx->codec_tag = 0;
            //正确设置sps/pps
            m_vEncodeCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
            //打开视频编码器
            ret = avcodec_open2(m_vEncodeCtx, encoder, nullptr);
            {
                QString error(QString("Can not open encoder id: %1").arg(encoder->id));
                RETBREAK(ret,error);
            }
            //将codecCtx中的参数传给输出流
            ret = avcodec_parameters_from_context(vStream->codecpar, m_vEncodeCtx);
            RETBREAK(ret,"Output avcodec_parameters_from_context");

        }
        //打开输出文件
        if (!(m_oFmtCtx->oformat->flags & AVFMT_NOFILE))
        {
            ret = avio_open(&m_oFmtCtx->pb, outFilePath.c_str(), AVIO_FLAG_WRITE);
            RETBREAK(ret,"avio_open failed");
        }
        //写文件头
        ret = avformat_write_header(m_oFmtCtx, &m_dict);
        {
            RETBREAK(ret,"avformat_write_header failed");
        }
        ret = 0;
    }while(0);

    return ret;
}

void ZWRecordThread::SetEncoderParm()
{
    m_vEncodeCtx->width = m_width;
    m_vEncodeCtx->height = m_height;
    m_vEncodeCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    m_vEncodeCtx->time_base.num = 1;
    m_vEncodeCtx->time_base.den = m_fps;
    m_vEncodeCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    QString suffix = QFileInfo(m_filePath).suffix();
    if (!QString::compare("mp4", suffix, Qt::CaseInsensitive) || !QString::compare("mkv", suffix, Qt::CaseInsensitive)
            || !QString::compare("mov", suffix, Qt::CaseInsensitive))
    {
        m_vEncodeCtx->codec_id = AV_CODEC_ID_H264;
        m_vEncodeCtx->bit_rate = 800 * 1000;
        m_vEncodeCtx->rc_max_rate = 800 * 1000;
        //codec_ctx->rc_min_rate = 200 * 1000;
        m_vEncodeCtx->rc_buffer_size = 500 * 1000;
        /* 设置图像组层的大小, gop_size越大，文件越小 */
        m_vEncodeCtx->gop_size = 30;
        m_vEncodeCtx->max_b_frames = 3;
        /* 设置h264中相关的参数 */
        m_vEncodeCtx->qmin = 10;	//2
        m_vEncodeCtx->qmax = 31;	//31
        m_vEncodeCtx->max_qdiff = 4;
        m_vEncodeCtx->me_range = 16;	//0
        m_vEncodeCtx->max_qdiff = 4;	//3
        m_vEncodeCtx->qcompress = 0.6;	//0.5
        av_dict_set(&m_dict, "profile", "high", 0);
        // 通过--preset的参数调节编码速度和质量的平衡。
        av_dict_set(&m_dict, "preset", "superfast", 0);
        av_dict_set(&m_dict, "threads", "0", 0);
        av_dict_set(&m_dict, "crf", "26", 0);
        // zerolatency: 零延迟，用在需要非常低的延迟的情况下，比如电视电话会议的编码
        av_dict_set(&m_dict, "tune", "zerolatency", 0);
        return;
    }
    else
    {
        m_vEncodeCtx->bit_rate = 4096 * 1000;
        if (!QString::compare("avi", suffix, Qt::CaseInsensitive))
            m_vEncodeCtx->codec_id = AV_CODEC_ID_MPEG4;
        else if (!QString::compare("wmv", suffix, Qt::CaseInsensitive))
            m_vEncodeCtx->codec_id = AV_CODEC_ID_MSMPEG4V3;
        else if (!QString::compare("flv", suffix, Qt::CaseInsensitive))
            m_vEncodeCtx->codec_id = AV_CODEC_ID_FLV1;
        else
            m_vEncodeCtx->codec_id = AV_CODEC_ID_MPEG4;
    }
}

void ZWRecordThread::Release()
{
    av_frame_free(&m_vOutFrame);
    av_free(m_vOutFrameBuf);

    if (m_vDecodeCtx)
    {
        avcodec_free_context(&m_vDecodeCtx);
        m_vDecodeCtx = nullptr;
    }
    if (m_vEncodeCtx)
    {
        avcodec_free_context(&m_vEncodeCtx);
        m_vEncodeCtx = nullptr;
    }
    if (m_vFifoBuf)
        av_fifo_freep(&m_vFifoBuf);
    if (m_vFmtCtx)
    {
        avformat_close_input(&m_vFmtCtx);
        m_vFmtCtx = nullptr;
    }
    avio_close(m_oFmtCtx->pb);
    avformat_free_context(m_oFmtCtx);
}

int ZWRecordThread::InitBuffer()
{
    ZW_LOG_TEST;
    if(!m_vEncodeCtx)
    {
        return -1;
    }
    m_vOutFrameSize = av_image_get_buffer_size(m_vEncodeCtx->pix_fmt, m_width, m_height, 1);
    m_vOutFrameBuf = (uint8_t *)av_malloc(m_vOutFrameSize);
    ZW_LOG_INFO(ZW_LOG_P4(m_width,m_height,m_vEncodeCtx->pix_fmt,m_vOutFrameSize));
    m_vOutFrame = av_frame_alloc();
    //先让AVFrame指针指向buf，后面再写入数据到buf
    av_image_fill_arrays(m_vOutFrame->data, m_vOutFrame->linesize, m_vOutFrameBuf, m_vEncodeCtx->pix_fmt, m_width, m_height, 1);
    //申请30帧缓存
    if (!(m_vFifoBuf = av_fifo_alloc_array(30, m_vOutFrameSize)))
    {
        ZW_LOG_WARNING("av_fifo_alloc_array failed");
        return -1;
    }
    return 0;
}

void ZWRecordThread::FlushEncoder()
{
    int ret = -11;
    AVPacket pkt = { 0 };
    av_init_packet(&pkt);
    ret = avcodec_send_frame(m_vEncodeCtx, nullptr);
    qDebug() << "avcodec_send_frame ret:" << ret;
    while (ret >= 0)
    {
        ret = avcodec_receive_packet(m_vEncodeCtx, &pkt);
        if (ret < 0)
        {
            av_packet_unref(&pkt);
            if (ret == AVERROR(EAGAIN))
            {
                qDebug() << "flush EAGAIN avcodec_receive_packet";
                continue;
            }
            else if (ret == AVERROR_EOF)
            {
                qDebug() << "flush video encoder finished";
                break;
            }
            qDebug() << "video avcodec_receive_packet failed, ret: " << ret;
            return;
        }
        pkt.stream_index = m_vOutIndex;
        av_packet_rescale_ts(&pkt, m_vEncodeCtx->time_base, m_oFmtCtx->streams[m_vOutIndex]->time_base);

        ret = av_interleaved_write_frame(m_oFmtCtx, &pkt);
        if (ret == 0)
            qDebug() << "flush Write video packet id: " << ++g_encodeFrameCnt_zw;
        else
            qDebug() << "video av_interleaved_write_frame failed, ret:" << ret;
        av_free_packet(&pkt);
    }
}

void ZWRecordThread::FlushDecoder()
{
    int ret = -1;
    AVPacket pkt = { 0 };
    av_init_packet(&pkt);
    int y_size = m_width * m_height;
    AVFrame	*oldFrame = av_frame_alloc();
    AVFrame *newFrame = av_frame_alloc();

    ret = avcodec_send_packet(m_vDecodeCtx, nullptr);
    qDebug() << "flush avcodec_send_packet, ret: " << ret;
    while (ret >= 0)
    {
        ret = avcodec_receive_frame(m_vDecodeCtx, oldFrame);
        if (ret < 0)
        {
            av_packet_unref(&pkt);
            if (ret == AVERROR(EAGAIN))
            {
                qDebug() << "flush EAGAIN avcodec_receive_frame";
                continue;
            }
            else if (ret == AVERROR_EOF)
            {
                qDebug() << "flush video decoder finished";
                break;
            }
            qDebug() << "flush video avcodec_receive_frame error, ret: " << ret;
            return;
        }
        ++g_collectFrameCnt_zw;
        sws_scale(m_swsCtx, (const uint8_t* const*)oldFrame->data, oldFrame->linesize, 0,
                  m_vEncodeCtx->height, newFrame->data, newFrame->linesize);

        {
            unique_lock<mutex> lk(m_mtx);
            m_cvNotFull.wait(lk, [this] {return av_fifo_space(m_vFifoBuf) >= m_vOutFrameSize; });
        }
        av_fifo_generic_write(m_vFifoBuf, newFrame->data[0], y_size, NULL);
        av_fifo_generic_write(m_vFifoBuf, newFrame->data[1], y_size / 4, NULL);
        av_fifo_generic_write(m_vFifoBuf, newFrame->data[2], y_size / 4, NULL);
        m_cvNotEmpty.notify_one();

    }
    qDebug() << "collect frame count: " << g_collectFrameCnt_zw;
}
