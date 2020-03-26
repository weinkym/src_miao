#ifndef ZWRECORDTHREAD_H
#define ZWRECORDTHREAD_H

#include <QThread>
#include <thread>
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

class ZWRecordThread : public QThread
{
    Q_OBJECT
public:
    enum RecordState {
        NotStarted,
        Started,
        Paused,
        Stopped,
        Unknown,
    };
    ZWRecordThread(QObject *parent = Q_NULLPTR);
    ~ZWRecordThread();
    void Start();
    void Pause();
    void Stop();

private:
    void ScreenRecordThreadProc();
    void ScreenAcquireThreadProc();

    int openVideo();
    int OpenOutput();
    void SetEncoderParm();
    void Release();
    int InitBuffer();
    void FlushEncoder();
    void FlushDecoder();

private:
    QString				m_filePath;
    int					m_width;
    int					m_height;
    int					m_fps;

    int m_vIndex;		//输入视频流索引
    int m_vOutIndex;	//输出视频流索引
    AVFormatContext		*m_vFmtCtx;
    AVFormatContext		*m_oFmtCtx;
    AVCodecContext		*m_vDecodeCtx;
    AVCodecContext		*m_vEncodeCtx;
    AVDictionary		*m_dict;
    SwsContext			*m_swsCtx;
    AVFifoBuffer		*m_vFifoBuf;
    AVFrame				*m_vOutFrame;
    uint8_t				*m_vOutFrameBuf;
    int					m_vOutFrameSize;	//一个输出帧的字节
    RecordState			m_state;

    //编码速度一般比采集速度慢，所以可以去掉m_cvNotEmpty
    std::condition_variable m_cvNotFull;
    std::condition_variable m_cvNotEmpty;
    std::mutex				m_mtx;
    std::condition_variable m_cvNotPause;
    std::mutex				m_mtxPause;
};

#endif // ZWRECORDTHREAD_H
