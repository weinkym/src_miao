#ifndef CPCMIODEVICE_H
#define CPCMIODEVICE_H

#include <QIODevice>
#include <QAudioFormat>

class CPCMIODevice : public QIODevice
{
    Q_OBJECT

public:
    CPCMIODevice(const QString &filePath,QObject *parent);
    ~CPCMIODevice();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;

private:
    qint64 m_pos;
    QByteArray m_buffer;
    QAudioFormat m_audioFormat;
};

#endif // CPCMIODEVICE_H
