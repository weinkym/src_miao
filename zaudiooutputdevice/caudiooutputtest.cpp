#include "caudiooutputtest.h"
#include <QDebug>

CAudioOutputTest::CAudioOutputTest(QObject *parent)
    :QObject(parent)
    ,m_audioOutput(NULL)
    ,m_PCMIODevice(NULL)
{

}

CAudioOutputTest::~CAudioOutputTest()
{
    if(m_PCMIODevice)
    {
        m_PCMIODevice->stop();
        m_PCMIODevice->deleteLater();
        m_PCMIODevice = NULL;
    }
}

void CAudioOutputTest::setVolume(qreal volume)
{
    if(m_audioOutput)
    {
        m_audioOutput->setVolume(volume);
    }
}

void CAudioOutputTest::start(const QString &filePath, const QAudioDeviceInfo &audioDeviceInfo)
{
    stop();

    if(m_PCMIODevice)
    {
        m_PCMIODevice->deleteLater();
        m_PCMIODevice = NULL;
    }
    m_PCMIODevice = new CPCMIODevice(filePath,this);
    connect(m_PCMIODevice,SIGNAL(readChannelFinished()),this,SLOT(onStateChanged()));

    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(audioDeviceInfo);
    if (!info.isFormatSupported(format)) {
//        qWarning() << "Default format not supported - trying to use nearest";
        format = info.nearestFormat(format);
    }

    if(m_audioOutput)
    {
        delete m_audioOutput;
        m_audioOutput = 0;
    }
    m_audioOutput = new QAudioOutput(audioDeviceInfo, format, this);
    connect(m_audioOutput,SIGNAL(stateChanged(QAudio::State)),this,SLOT(onStateChanged()));

    if(m_PCMIODevice)
    {
        m_PCMIODevice->start();
    }
    m_audioOutput->start(m_PCMIODevice);
}

void CAudioOutputTest::stop()
{
    if(m_audioOutput)
    {
        m_audioOutput->stop();
    }

    if(m_PCMIODevice)
    {
        m_PCMIODevice->stop();
    }
}

void CAudioOutputTest::onStateChanged()
{
    qDebug()<<"onReadChannelFinished"<<Q_FUNC_INFO<<m_audioOutput->state();
    if(m_audioOutput->state() == QAudio::IdleState)
    {
        stop();
        emit sigFinished();
    }
}

