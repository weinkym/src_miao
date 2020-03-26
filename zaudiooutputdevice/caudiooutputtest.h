#ifndef CAUDIOOUTPUTTEST_H
#define CAUDIOOUTPUTTEST_H

#include <QAudioOutput>
#include "cpcmiodevice.h"

class CAudioOutputTest : public QObject
{
    Q_OBJECT

public:
    CAudioOutputTest(QObject *parent = Q_NULLPTR);
    ~CAudioOutputTest();
    void setVolume(qreal volume);
    void start(const QString &filePath,const QAudioDeviceInfo &audioDeviceInfo);

private:
    void stop();

private slots:
    void onStateChanged();

signals:
    void sigFinished();

private:
    QAudioOutput *m_audioOutput;
    CPCMIODevice *m_PCMIODevice;
};

#endif // CAUDIOOUTPUTTEST_H
