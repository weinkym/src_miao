#include "zwvideothread.h"
#include <QImage>
#include "zwlog.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
}

ZWVideoThread::ZWVideoThread(QObject *parent)
    :QThread(parent)
    ,m_pause(false)
{
    m_filePath = "/Users/miaozw/Movies/test.mp4";
}

ZWVideoThread::~ZWVideoThread()
{

}

void ZWVideoThread::run()
{
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    int ret = avformat_open_input(&pFormatCtx,m_filePath.toLocal8Bit().data(),NULL,NULL);
    if(ret != 0)
    {
        emit sigErrorHappened(ret,"avformat_open_input");
        return;
    }
    ret = avformat_find_stream_info(pFormatCtx,NULL);
    if(ret != 0)
    {
        ZW_LOG_WARNING(QString("avformat_find_stream_info is error"));
        return;
    }

    ZW_LOG_INFO(QString("nb_streams=%1").arg(pFormatCtx->nb_streams));

    int videoStreamIndex = -1;
    //循环查找视频中包含的流信息，直到找到视频类型的流
    //便将其记录下来 保存到videoStream变量中
    //这里我们现在只处理视频流  音频流先不管他
    for (int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex = i;
        }
    }
    if(videoStreamIndex == -1)
    {
        emit sigErrorHappened(videoStreamIndex,"can not found AVMEDIA_TYPE_VIDEO");
        return;
    }
    ZW_LOG_INFO(QString("videoStreamIndex=%1").arg(videoStreamIndex));
    AVCodecContext * pCodecCtx = pFormatCtx->streams[videoStreamIndex]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL)
    {
        emit sigErrorHappened(-1,"avcodec is null");
        return;
    }
    ret = avcodec_open2(pCodecCtx,pCodec,NULL);
    if(ret < 0)
    {
        emit sigErrorHappened(ret,"avcodec_open2");
        return;
    }
    ZW_LOG_INFO(QString("pCodecCtx,width=%1,height=%2,pix_fmt=%3")
                .arg(pCodecCtx->width)
                .arg(pCodecCtx->height)
                .arg(pCodecCtx->pix_fmt));
    AVFrame *pFrame = av_frame_alloc();
    AVFrame *pFrameRGB = av_frame_alloc();
    SwsContext *img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                                     pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                                     AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

    int y_size = pCodecCtx->width * pCodecCtx->height;
    AVPacket *pPacket = (AVPacket*)malloc(sizeof(AVPacket));
    ret = av_new_packet(pPacket,y_size);

    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);
    uint8_t *out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_RGB24,
                   pCodecCtx->width, pCodecCtx->height);

    while(1)
    {
        if(m_pause)
        {
            msleep(10);
            continue;
        }
        if(av_read_frame(pFormatCtx,pPacket) < 0)
        {
            break;
        }
        if(pPacket->stream_index == videoStreamIndex)
        {
            int got_picture = -1;
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,pPacket);
            if(ret < 0)
            {
                emit sigErrorHappened(ret,"avcodec_decode_video2");
                break;
            }
            if(got_picture)
            {
                sws_scale(img_convert_ctx,
                          (uint8_t const * const *) pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                          pFrameRGB->linesize);

                QImage image(pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB888);
                for(int y = 0; y < pCodecCtx->height; ++y)
                {
                    for(int x = 0; x < pCodecCtx->width; ++x)
                    {
                        int r = (pFrameRGB->data[0]+y*pFrameRGB->linesize[0])[3*x];
                        int g = (pFrameRGB->data[0]+y*pFrameRGB->linesize[0])[3*x + 1];
                        int b = (pFrameRGB->data[0]+y*pFrameRGB->linesize[0])[3*x + 2];
                        image.setPixelColor(x,y,QColor(r,g,b));
                    }
                }
                emit sigImageChanged(image);
            }
        }
        av_free_packet(pPacket);
        //TODOtest 防止cpu过高
        msleep(40);
    }
    av_free(out_buffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    ZW_LOG_INFO("TTTTTTTTTT");
}

void ZWVideoThread::pause()
{
    m_pause = true;
}

void ZWVideoThread::play()
{
    m_pause = false;
}
