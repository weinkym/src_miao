#include "cdownloadsettings.h"
#include "clogsetting.h"
#include <QDebug>
#include <QFile>
#include <QSettings>

CDownloadSettings::CDownloadSettings(const QString &filePath)
    : m_fileSize(0)
    , m_chunkSize(1024 * 1024)
    , m_threadCount(4)
    , m_rangeData(0, 0)
    , m_filePath(filePath)
    , m_timestamp("")
{
    do
    {
        QSettings settings(getSettingsFile(m_filePath), QSettings::IniFormat);
        m_chunkSize = settings.value(QString("chunkSize"), 0).toULongLong();
        m_fileSize = settings.value(QString("fileSize"), 0).toULongLong();
        m_timestamp = settings.value(QString("timestamp")).toByteArray();
        m_rangeData = CRangeData(settings.value(QString("rangData")).toString());
    } while(0);
}

//CDownloadSettings::CDownloadSettings(const QString &saveFilePath, quint64 fileSize, const QByteArray &timestamp)
//    : m_isValid(false)
//    , m_fileSize(fileSize)
//    , m_chunkSize(1024 * 1024)
//    , m_threadCount(4)
//    , m_rangeData(0, 0)
//    , m_saveFilePath(saveFilePath)
//    , m_filePath(saveFilePath + "cfg.ini")
//    , m_timestamp(timestamp)

//{
//    do
//    {
//        if(m_chunkSize < fileSize)
//        {
//            m_chunkSize = fileSize;
//        }
//        if(fileSize > 0)
//        {
//            m_rangeData = CRangeData(0, getChunkCount(m_fileSize, m_chunkSize) - 1);
//        }
//    } while(0);
//}

void CDownloadSettings::update(const QString &filePath, quint64 fileSize, const QByteArray &timestamp)
{
    C_LOG_OUT_V6(fileSize, m_fileSize, m_filePath, filePath, m_timestamp, timestamp);
    if(fileSize != m_fileSize || m_timestamp != timestamp || m_filePath != filePath || !isValid())
    {
        m_filePath = filePath;
        m_fileSize = fileSize;
        m_chunkSize = 1024 * 1024;
        //    int m_threadCount;
        m_timestamp = timestamp;
        updateRangeData();
    }
}

CDownloadSettings::~CDownloadSettings()
{
}

bool CDownloadSettings::isValid() const
{
    //todo file path md5 验证
    //ini last mod 验证
    bool isValid = false;
    do
    {
        if(!QFile::exists(m_filePath))
        {
            break;
        }
        if(m_chunkSize <= 0)
        {
            break;
        }
        if(m_fileSize <= 0)
        {
            break;
        }
        if(!m_rangeData.isValid())
        {
            break;
        }
        QFile info(m_filePath);
        if(info.size() != m_fileSize)
        {
            break;
        }
        isValid = m_rangeData.getMax() <= (getChunkCount(m_fileSize, m_chunkSize) - 1);
    } while(0);

    return isValid;
}

quint64 CDownloadSettings::getChunkCount(quint64 fileSize, quint64 chunkSize)
{
    //余数合并到最后
    if(chunkSize == 0)
    {
        return 0;
    }
    if(fileSize == 0)
    {
        return 0;
    }
    if(fileSize <= chunkSize)
    {
        return 1;
    }
    return fileSize / chunkSize;
}

qint64 CDownloadSettings::writeToFile(quint64 index, const QByteArray &data)
{
    QFile file(m_filePath);
    if(file.open(QIODevice::ReadWrite))
    {
        file.seek(index * m_chunkSize);
        qint64 size = file.write(data);
        file.close();
        m_rangeData.remove(index);
        saveSettings();
        qDebug() << __LINE__ << __FUNCTION__ << index << size;
        return size;
    }
    return -1;
}

bool CDownloadSettings::isFinished() const
{
    return isValid() && m_rangeData.isFinished();
}

int CDownloadSettings::getChunkSize() const
{
    return m_chunkSize;
}

quint64 CDownloadSettings::getMax() const
{
    return m_rangeData.getMax();
}

quint64 CDownloadSettings::getIndexChunkSize(quint64 index) const
{
    if(index > getMax())
    {
        return 0;
    }
    quint64 chunkSize = m_chunkSize;
    if(index == getMax())
    {
        chunkSize += m_fileSize % m_chunkSize;
    }
    return chunkSize;
}

quint64 CDownloadSettings::getIndexPos(quint64 index, bool &ok) const
{
    if(index > getMax() || index < 0)
    {
        ok = false;
        return 0;
    }
    ok = true;
    quint64 chunkSize = m_chunkSize * index;
    return chunkSize;
}

quint64 CDownloadSettings::getFreeIndex(bool &ok)
{
    return m_rangeData.getFreeIndex(ok);
}

QString CDownloadSettings::getSettingsFile(const QString &filePath)
{
    return filePath + ".cfg.ini";
}

void CDownloadSettings::saveSettings()
{
    QSettings settings(getSettingsFile(m_filePath), QSettings::IniFormat);
    settings.setValue("chunkSize", m_chunkSize);
    settings.setValue("fileSize", m_fileSize);
    settings.setValue("timestamp", m_timestamp);
    settings.setValue("rangData", m_rangeData.toFormatString());
    settings.sync();
}

void CDownloadSettings::updateRangeData()
{
    if(m_chunkSize > m_fileSize)
    {
        m_chunkSize = m_fileSize;
    }
    if(m_fileSize > 0)
    {
        m_rangeData = CRangeData(0, getChunkCount(m_fileSize, m_chunkSize) - 1);
    }
    else
    {
        m_rangeData = CRangeData(0, 0);
    }
}
