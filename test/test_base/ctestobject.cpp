#include "ctestobject.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
const int chunk_size = 1024;

static bool createTempile(const QString &filePath, quint64 size)
{
    qDebug() << __LINE__ << __FUNCTION__;
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly))
    {
        QByteArray data(size, '\0');
        qint64 writeSize = file.write(data);
        file.close();
        qDebug() << __LINE__ << __FUNCTION__ << writeSize;
        return writeSize == size;
    }
    return false;
}

CTestObject::CTestObject(const QString &filePath, QObject *parent)
    : QObject(parent)
    , m_downloadSettings(NULL)
    , m_filePath(filePath)
{
    QFileInfo info(filePath);
    m_saveFilePath = QString("%1/TT_%2").arg(info.absolutePath()).arg(info.fileName());
    qDebug() << __LINE__ << __FUNCTION__ << m_saveFilePath;
    m_downloadSettings = new CDownloadSettings(m_saveFilePath, info.size(), "");
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    m_timer.setInterval(1000);
    m_timer.setSingleShot(true);

    for(quint64 i = 0; i <= m_downloadSettings->getMax(); ++i)
    {
        m_indexList.append(i);
    }
}

CTestObject::~CTestObject()
{
}

void CTestObject::start()
{
    if(m_timer.isActive())
    {
        return;
    }
    QFileInfo info(m_filePath);
    createTempile(m_saveFilePath, info.size());
    m_timer.start();
}

void CTestObject::onTimeout()
{
    if(m_indexList.isEmpty())
    {
        return;
    }
    int index = qrand() % m_indexList.count();
    m_indexList.removeAt(index);
    QFile file(m_filePath);
    qint64 pos = m_downloadSettings->getIndexPos(index);
    qint64 readSize = m_downloadSettings->getIndexChunkSize(index);
    if(file.open(QIODevice::ReadOnly) && file.seek(pos))
    {
        QByteArray data = file.read(readSize);
        m_downloadSettings->writeToFile(index, data);
        file.close();
    }
    if(m_indexList.isEmpty())
    {
        emit sigFinished(true);
        return;
    }
    m_timer.start();
}
