#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QImage>

#include "zwcore.h"
#include "zwlog.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
}

///现在我们需要做的是让SaveFrame函数能把RGB信息定稿到一个PPM格式的文件中。
///我们将生成一个简单的PPM格式文件，请相信，它是可以工作的。
void SaveFrame(AVFrame *pFrame, int width, int height,int index)
{

  FILE *pFile;
  char szFilename[32];
  int  y;

  // Open file
  sprintf(szFilename, "frame%d.ppm", index);
//  pFile=fopen(szFilename, "wb");
  pFile=fopen("/Users/miaozw/Movies/test.png", "wb");

  if(pFile==NULL)
    return;

  // Write header
  fprintf(pFile, "P6%d %d255", width, height);

  // Write pixel data
  QImage image(width,height,QImage::Format_ARGB32);
  ZW_LOG_INFO(QString("width=%1,height=%2").arg(width).arg(height));
  for(y=0; y<height; y++)
  {
//    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
      for(int x = 0; x < width;++x)
      {
//        printf("%d ",(pFrame->data[0]+y*pFrame->linesize[0])[x]);
          int r = (pFrame->data[0]+y*pFrame->linesize[0])[3*x];
          int g = (pFrame->data[0]+y*pFrame->linesize[0])[3*x + 1];
          int b = (pFrame->data[0]+y*pFrame->linesize[0])[3*x + 2];
        image.setPixelColor(x,y,QColor(r,g,b));
      }
//    printf("\n");
  }
  image.save("/Users/miaozw/Movies/temp.png");

  // Close file
  fclose(pFile);

}

void test()
{
    unsigned version = avcodec_version();
    qDebug() << "version is:" << version;
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    char *file_path = "/Users/miaozw/Movies/test.mp4";
    int ret = avformat_open_input(&pFormatCtx, file_path, NULL, NULL);
    ZW_LOG_INFO(QString("result=%1").arg(ret));
    if(ret != 0)
    {
        ZW_LOG_WARNING(QString("avformat_open_input is error"));
        return;
    }
    ZW_LOG_INFO("TTTTTTTTTT");
//    AVFormatContext *context = avformat_alloc_context();
    ZW_LOG_INFO("TTTTTTTTTT");
    ret = avformat_find_stream_info(pFormatCtx,NULL);
    ZW_LOG_INFO("TTTTTTTTTT");
    if(ret != 0)
    {
        ZW_LOG_WARNING(QString("avformat_find_stream_info is error"));
        return;
    }
    ZW_LOG_INFO("TTTTTTTTTT");

    int videoStream = -1;
    //循环查找视频中包含的流信息，直到找到视频类型的流
    //便将其记录下来 保存到videoStream变量中
    //这里我们现在只处理视频流  音频流先不管他
    for (int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
        }
    }
    if (videoStream == -1)
    {
        ZW_LOG_WARNING(QString("videoStream is error"));
        return;
    }
    ZW_LOG_INFO(QString("TTTTTTTTTTvideoStreamIndex=%1").arg(videoStream));
    //查找解码器
    AVCodecContext *pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL)
    {
        ZW_LOG_WARNING(QString("pCodec is null"));
        return;
    }
    //打开解码器
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        ZW_LOG_WARNING(QString("avcodec_open2"));
        return;
    }
    ZW_LOG_INFO("TTTTTTTTTT");

    AVFrame *pFrame = av_frame_alloc();
    AVFrame *pFrameRGB = av_frame_alloc();
    ZW_LOG_INFO("TTTTTTTTTT");

    ZW_LOG_INFO(QString("TTTTTTTTTT,%1,%2,%3,%4,%5,%6")
                .arg(pCodecCtx->width)
                .arg(pCodecCtx->height)
                .arg(pCodecCtx->pix_fmt)
                .arg(pCodecCtx->width)
                .arg(pCodecCtx->width)
                .arg(pCodecCtx->height));

    static struct SwsContext *img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                                     pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                                     AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

    ZW_LOG_INFO("TTTTTTTTTT");
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);
    ZW_LOG_INFO("TTTTTTTTTT");

    uint8_t *out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_RGB24,
                   pCodecCtx->width, pCodecCtx->height);

    int y_size = pCodecCtx->width * pCodecCtx->height;

    AVPacket *packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
    av_new_packet(packet, y_size); //分配packet的数据

    av_dump_format(pFormatCtx, 0, file_path, 0); //输出视频信息

    int index = 0;
    int got_picture;

    ZW_LOG_INFO("TTTTTTTTTT");
    while (1)
    {
        if (av_read_frame(pFormatCtx, packet) < 0)
        {
            break; //这里认为视频读取完了
        }

        if (packet->stream_index == videoStream) {
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);

            if (ret < 0)
            {
                ZW_LOG_WARNING(QString("decode error."));
                return;
            }

            if (got_picture) {
                sws_scale(img_convert_ctx,
                          (uint8_t const * const *) pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                          pFrameRGB->linesize);

                SaveFrame(pFrameRGB, pCodecCtx->width,pCodecCtx->height,index++); //保存图片
                ZW_LOG_INFO("TTTTTTTTTT");
//                for(int i = 0; i < pCodecCtx->height;++i)
//                {
//                    for(int j = 0; j < pFrameRGB->linesize[i];++j)
//                    {
//                        printf("%d ",pFrameRGB->data[j + i * pFrameRGB->linesize[0]]);
//                    }
//                    printf("\n");
//                }
                break;
                //                    if (index > 50) return 0; //这里我们就保存50张图片
            }
        }
        av_free_packet(packet);
    }
    av_free(out_buffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    av_register_all();

//    test();
//    return -1;
    MainWindow w;
    w.show();

    return a.exec();
}
