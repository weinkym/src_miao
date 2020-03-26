#include "cpcmiodevice.h"
#include <QFile>

CPCMIODevice::CPCMIODevice(const QString &filePath, QObject *parent)
    :QIODevice(parent)
    ,m_pos(0)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        m_buffer = file.read(4 * 1024 * 1024);
        file.close();
    }
}

CPCMIODevice::~CPCMIODevice()
{

}

void CPCMIODevice::start()
{
    open(QIODevice::ReadOnly);
}

void CPCMIODevice::stop()
{
    close();
    m_pos = 0;
}

qint64 CPCMIODevice::readData(char *data, qint64 maxLen)
{
    qint64 total = 0;
    if (!m_buffer.isEmpty() && m_pos < m_buffer.size() && isOpen()) {
            const qint64 chunk = qMin((m_buffer.size() - m_pos), maxLen);
            memcpy(data + total, m_buffer.constData() + m_pos, chunk);
            m_pos = (m_pos + chunk);
            total += chunk;
    }
    return total;
}

qint64 CPCMIODevice::writeData(const char *data, qint64 len)
{
    return 0;
}

qint64 CPCMIODevice::bytesAvailable() const
{
    return m_buffer.size() - m_pos;
}
