#include "mainwindow.h"
#include <QApplication>
#include "zwlog.h"

int main(int argc, char *argv[])
{
//    LOG_TYPE_FILE = 0x00000001,
//    LOG_TYPE_DATE = LOG_TYPE_FILE<<1,
//    LOG_TYPE_DATE_TIME = LOG_TYPE_DATE<<1,
//    LOG_TYPE_DATE_TIMES = LOG_TYPE_DATE_TIME<<1,
//    LOG_TYPE_THREAD = LOG_TYPE_DATE_TIMES<<1,
//    LOG_TYPE_LINE = LOG_TYPE_THREAD<<1,
//    LOG_TYPE_FUNCTION = LOG_TYPE_LINE<<1,
//    LOG_TYPE_LEVEL_STRING = LOG_TYPE_FUNCTION<<1,
    QApplication a(argc, argv);
    ZW_LOG::setLogTypes(ZW_LOG_TYPES(LOG_TYPE_DATE | LOG_TYPE_LINE | LOG_TYPE_FUNCTION));
    MainWindow w;
    w.show();

    return a.exec();
}
//#include <iostream>
//extern "C" {
//#include "libavcodec/avcodec.h"
//#include "libavformat/avformat.h"
//#include <libavdevice/avdevice.h>
//}

//using namespace std;

//int main(int argc, const char *argv[]) {
//  int ret = 0;
//  int videoStreamIndex = -1;
//  AVFormatContext *s = NULL;
//  AVDictionary *options = NULL;
//  AVCodecParameters *par = NULL;
//  AVCodec *codec;
//  AVCodecContext *cctx;

//  av_register_all();
//  avdevice_register_all();

//  AVInputFormat *ifmt = av_find_input_format("avfoundation");

//  av_dict_set(&options, "framerate", "30", 0);
//  av_dict_set(&options, "video_size", "1280x720", 0);
//  ret = avformat_open_input(&s, "0", ifmt, &options);
//  if (ret < 0) {
//    cout << av_err2str(ret) << endl;
//    goto error;
//  }

//  ret = avformat_find_stream_info(s, NULL);
//  if (ret < 0) {
//    cout << av_err2str(ret) << endl;
//    goto error;
//  }

//  for (uint i = 0; i < s->nb_streams; i++) {
//    if (s->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
//      videoStreamIndex = i;
//      break;
//    }
//  }

//  if (-1 == videoStreamIndex) {
//    cout << "can't find video stream" << endl;
//    goto error;
//  }

//  par = s->streams[videoStreamIndex]->codecpar;
//  codec = avcodec_find_decoder(par->codec_id);
//  cctx = avcodec_alloc_context3(codec);
//  avcodec_parameters_to_context(cctx, par);

//  ret = avcodec_open2(cctx, codec, NULL);
//  if (ret < 0) {
//    cout << av_err2str(ret) << endl;
//    goto error;
//  }

//error:
//  if (s) {
//    avformat_free_context(s);
//  }

//  if (cctx) {
//    avcodec_free_context(&cctx);
//  }

//  if (options) {
//    av_dict_free(&options);
//  }

//  return ret;
//}
