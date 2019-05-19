#include "zwffpublic.h"
#include <QStringList>

#ifdef	__cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavutil/fifo.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
//#include
#ifdef __cplusplus
};
#endif

ZWFFPublic::ZWFFPublic()
{

}

QStringList ZWFFPublic::get_input_format_list()
{
    QStringList objList;
    AVInputFormat *fmt = NULL;
    fmt = av_iformat_next(fmt);
    while(fmt)
    {
        objList.append(fmt->name);
        fmt = av_iformat_next(fmt);
    }
    return objList;
}
