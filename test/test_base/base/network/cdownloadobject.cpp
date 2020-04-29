#include "cdownloadobject.h"
#include "cdownloadrequestaction.h"
#include "cdownloadsettings.h"
#include "clogsetting.h"
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPointer>
#include <QSettings>
#include <QTimer>

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

CDownloadObject::CDownloadObject(const QString &saveFilePath, const QString &url, QObject *parent)
    : CBaseRequester(parent)
    , m_downloadSettings(new CDownloadSettings(saveFilePath))
    , m_cancel(false)
    , m_status(STATUS_NORMAL)
    , m_url(url)
    , m_filePath(saveFilePath)
{
}

CDownloadObject::~CDownloadObject()
{
    if(m_downloadSettings)
    {
        delete m_downloadSettings;
        m_downloadSettings = NULL;
    }
}

void CDownloadObject::start()
{
    if(m_status == STATUS_STARTINGT)
    {
        return;
    }

    if(m_url.isEmpty())
    {
        doError("url is empty");
        return;
    }
    if(m_filePath.isEmpty())
    {
        doError("filePath is empty");
        return;
    }

    setStatus(STATUS_REQUEST_HEAD);
}

void CDownloadObject::cancel()
{
    C_LOG_FUNCTION;
    m_cancel = true;
    this->stopReqeust(m_headRequestUuid);
    QHashIterator<QUuid, int> iter(m_partUuidMap);
    while(iter.hasNext())
    {
        iter.next();
        this->stopReqeust(iter.key());
    }
    setStatus(STATUS_CANCEL);
}

void CDownloadObject::setStatus(Status status)
{
    C_LOG_FUNCTION;
    C_LOG_OUT_V2(status, m_status);
    if(m_status == status)
    {
        return;
    }
    m_status = status;
    emit sigStateUpdated(m_status);
    switch(m_status)
    {
    case STATUS_REQUEST_HEAD:
    {
        requestHead();
        break;
    }
    case STATUS_FINISHED:
    {
        QString iniFilePath = m_downloadSettings->getSettingsFile();
        if(QFile::exists(iniFilePath))
        {
            bool remvoeOk = QFile::remove(iniFilePath);
            C_LOG_OUT_V2(remvoeOk, iniFilePath);
        }
        break;
    }
    default:
        break;
    }
}

void CDownloadObject::touchFile(const QString &filename)
{
    C_LOG_FUNCTION;
    QFile f(filename);
    f.open(QIODevice::ReadWrite);
    f.close();
}

void CDownloadObject::doRequestFinished(bool ok, const CBaseRequestAction::Data &data)
{
    if(!ok)
    {
        cancel();
        return;
    }
    if(m_headRequestUuid == data.uuid)
    {
        bool needRequestAll = true;
        quint64 fileSize = data.rawHeaderMap.value("Content-Length").toULongLong();
        C_LOG_OUT_V(fileSize);
        if(fileSize > 0)
        {
            if(!QFile::exists(m_filePath))
            {
                createTempile(m_filePath, fileSize);
            }
            needRequestAll = false;
            m_downloadSettings->update(m_filePath, fileSize, data.rawHeaderMap.value("Last-Modified"));
            if(m_downloadSettings->isFinished())
            {
                setStatus(STATUS_FINISHED);
            }
            else if(m_downloadSettings->isValid())
            {
                requestPartial();
            }
            else
            {
                needRequestAll = true;
            }
        }
        if(needRequestAll)
        {
            //TODO
            //            request()
        }
        C_LOG_OUT_V(needRequestAll);
        return;
    }
    if(m_partUuidMap.contains(data.uuid))
    {
        m_downloadSettings->writeToFile(m_partUuidMap.value(data.uuid), data.replyArray);
        m_partUuidMap.remove(data.uuid);
    }
    if(m_downloadSettings->isFinished())
    {
        setStatus(STATUS_FINISHED);
    }
    else
    {
        requestPartial();
    }
    emit sigProcessUpdated(m_downloadSettings->getDownloadSize(), m_downloadSettings->getFileSize());
}

void CDownloadObject::requestHead()
{
    CDownloadRequestAction *action = CDownloadRequestAction::createHeadAction(m_url);
    m_headRequestUuid = this->request(action);
}

void CDownloadObject::doError(const QString &error)
{
    C_LOG_WARNING(QString("download is error,%1").arg(C_LOG_P(error)));
    m_lastError = error;
    setStatus(STATUS_FAIL);
}

void CDownloadObject::requestPartial()
{
    C_LOG_FUNCTION;
    bool ok = false;
    do
    {
        quint64 index = m_downloadSettings->getFreeIndex(ok);
        C_LOG_OUT_V2(index, ok);
        if(!ok)
        {
            break;
        }
        quint64 size = m_downloadSettings->getIndexChunkSize(index);
        C_LOG_OUT_V2(size, ok);
        if(size == 0)
        {
            break;
        }
        quint64 pos = m_downloadSettings->getIndexPos(index, ok);
        C_LOG_OUT_V2(pos, ok);
        if(!ok)
        {
            break;
        }
        if(ok)
        {
            CDownloadRequestAction *action = CDownloadRequestAction::createPartAction(m_url, pos, pos + size - 1);
            QUuid uuid = this->request(action);
            C_LOG_OUT_V4(uuid, index, pos, size);
            m_partUuidMap.insert(uuid, index);
        }
    } while(ok && m_partUuidMap.count() < 4);
}
