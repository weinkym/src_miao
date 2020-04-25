#include "cdownloadobject.h"
#include "clogsetting.h"
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QTimer>

const QString CDownloadObject::m_tmpSuffix = QLatin1String(".tmp");

CDownloadObject::CDownloadObject(const QString &saveFilePath, const QString &url, QNetworkAccessManager *networkAccessManager, QObject *parent)
    : QObject(parent)
    , m_networkAccessMgr(networkAccessManager)
    , m_reply(NULL)
    , m_timeoutTimer(NULL)
    , m_status(STATUS_NORMAL)
    , m_url(url)
    , m_filePath(saveFilePath)
{
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setInterval(45 * 1000);
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(onRequestTimeout()));
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
    if(!m_networkAccessMgr)
        m_networkAccessMgr = new QNetworkAccessManager(this);

    QString settingsFile = m_filePath + ".cfg.ini";
    QString tmpFileName = m_filePath + ".tmp";
    m_downloadInfo = DownloadInfo(settingsFile);

    //todo 修改后重新下载逻辑
}

void CDownloadObject::downloadFile(const QString &filePath, const QString &url, bool fullDownload)
{
    //    C_LOG_VALUE3(C_LOG_LEVEL_INFO, "download file", filePath, url, m_status);
    //    if(m_status == CPB::TRANSFER_STATE_DOING)
    //    {
    //        return;
    //    }
    //    setStatus(CPB::TRANSFER_STATE_DOING);
    //    if(url.isEmpty())
    //    {
    //        C_LOG_ERROR(QString("Downloadfile url is empty."));
    //        return;
    //    }
    //    if(filePath.isEmpty())
    //    {
    //        C_LOG_ERROR(QString("Downloadfile filePath is empty."));
    //        return;
    //    }
    //    m_url = url;
    //    m_filePath = filePath;

    //    if(fullDownload)
    //    {
    //        if(!m_networkAccessMgr)
    //            m_networkAccessMgr = new QNetworkAccessManager(this);

    //        QNetworkRequest req(m_url);
    //        m_reply = m_networkAccessMgr->get(req);
    //        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
    //            this, SLOT(onDownloadNetworkError(QNetworkReply::NetworkError)));
    //        connect(m_reply, SIGNAL(finished()), this, SLOT(onFullDownloadFinished()));
    //        return;
    //    }

    //    QString settingsFile = getSettingsFile();
    //    QString localFileName = filePath;
    //    QString tmpFileName = getLocalTmpFile();
    //    bool isDownloadFileExist = QFile::exists(filePath);
    //    bool isTmpFileExist = QFile::exists(tmpFileName);

    //    m_downloadInfo = DownloadInfo(settingsFile);

    //    //程序有下载过
    //    QFileInfo fileInfo(tmpFileName);
    //    quint64 tmpFileSize = fileInfo.size();
    //    quint64 secsTo = qAbs(fileInfo.lastModified().secsTo(m_downloadInfo.lastTime));

    //    C_LOG_TEST(QString("fileInfo.lastModified()=%1").arg(fileInfo.lastModified().toString("hh:mm:ss")));
    //    C_LOG_TEST(QString("m_downloadInfo.lastTime=%1").arg(m_downloadInfo.lastTime.toString("hh:mm:ss")));
    //    C_LOG_TEST(QString("m_complete=%1,m_fileSize=%2,isDownloadFileExist=%3,isTmpFileExist=%4,tmpFileSize=%5,secsTo=%6")
    //                   .arg(m_downloadInfo.complete)
    //                   .arg(m_downloadInfo.fileSize)
    //                   .arg(isDownloadFileExist ? "yes" : "no")
    //                   .arg(isTmpFileExist ? "YES" : "NO")
    //                   .arg(tmpFileSize)
    //                   .arg(secsTo));

    //    //TODO: >= is temp solution, to avoid some bug
    //    if(m_downloadInfo.complete >= m_downloadInfo.fileSize && m_downloadInfo.fileSize > 0 && isDownloadFileExist)
    //    {
    //        setStatus(CPB::TRANSFER_STATE_SUCCESS);
    //        emit sigProcessUpdated(m_downloadInfo.complete, m_downloadInfo.fileSize);
    //        return;
    //    }
    //    setStatus(CPB::TRANSFER_STATE_DOING);

    //    //误差超过10s重新下载
    //    if(secsTo > 10 || m_downloadInfo.complete == 0
    //        || m_downloadInfo.fileSize == 0
    //        || tmpFileSize != m_downloadInfo.complete)
    //    {
    //        //从头开始下载
    //        //删除所有下载相关文件，从头下载
    //        if(isTmpFileExist)
    //            QFile::remove(tmpFileName);
    //        if(isDownloadFileExist)
    //            QFile::remove(localFileName);
    //        QFile::remove(settingsFile);

    //        m_downloadInfo.complete = 0;
    //        m_downloadInfo.fileSize = 0;
    //        m_downloadInfo.lastTime = QDateTime();
    //        m_downloadInfo.timestamp = QByteArray();
    //        m_downloadInfo.save();
    //        emit sigProcessUpdated(m_downloadInfo.complete, m_downloadInfo.fileSize);
    //        sendHttpHeadRequest();
    //    }
    //    else
    //    {
    //        emit sigProcessUpdated(m_downloadInfo.complete, m_downloadInfo.fileSize);
    //        sendHttpPartialGetRequest();
    //    }
}

void CDownloadObject::cancel()
{
    C_LOG_FUNCTION;
    //    m_status = CPB::TRANSFER_STATE_CANCEL;
}

void CDownloadObject::setStatus(Status status)
{
    C_LOG_FUNCTION;
    C_LOG_OUT_V2(status, m_status);
    if(m_status == status)
    {
        return;
    }
    m_timeoutTimer->stop();
    m_status = status;
    emit sigStateUpdated(m_status);
}

void CDownloadObject::sendHttpHeadRequest()
{
    C_LOG_FUNCTION;
    //    if(m_status == CPB::TRANSFER_STATE_CANCEL)
    //    {
    //        return;
    //    }

    //    if(m_networkAccessMgr == NULL)
    //    {
    //        m_networkAccessMgr = new QNetworkAccessManager(this);
    //    }

    QUrl url = QUrl(m_url);
    QNetworkRequest req(url);
    req.setRawHeader("Accept-Encoding", "none");
    //    req.setHeader(QNetworkRequest::CookieHeader, CBizUtil::getLoginCookies());
    m_reply = m_networkAccessMgr->head(req);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onHttpFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onDownloadNetworkError(QNetworkReply::NetworkError)));

    if(m_timeoutTimer->isActive())
    {
        m_timeoutTimer->stop();
    }
    m_timeoutTimer->start();
}

void CDownloadObject::sendHttpPartialGetRequest()
{
    C_LOG_FUNCTION;
    //    if(m_status == CPB::TRANSFER_STATE_CANCEL)
    //    {
    //        return;
    //    }

    //    if(m_downloadInfo.complete >= m_downloadInfo.fileSize)
    //    {
    //        C_LOG_WARNING(QString("Invalid args, complete = %1, filesize = %2").arg(m_downloadInfo.complete).arg(m_downloadInfo.fileSize));
    //        return;
    //    }

    //    QUrl url = QUrl(m_url);
    //    QNetworkRequest req(m_url);
    //    C_LOG_DEBUG(QString("complete=%1,filesize=%2,CHUNK_SIZE=%3, url:=%4").arg(m_downloadInfo.complete).arg(m_downloadInfo.fileSize).arg(CHUNK_SIZE).arg(m_url));

    //    quint64 finishedDownloadSize = m_downloadInfo.complete + m_dataCache.size();
    //    if(finishedDownloadSize + CHUNK_SIZE > m_downloadInfo.fileSize)
    //    {
    //        QString s = QString("bytes=%1-%2").arg(finishedDownloadSize).arg(QString(""));
    //        req.setRawHeader("Range", s.toUtf8());
    //        C_LOG_DEBUG(QString("Range:%1").arg(s));
    //    }
    //    else
    //    {
    //        QString s = QString("bytes=%1-%2").arg(finishedDownloadSize).arg(finishedDownloadSize + CHUNK_SIZE - 1);
    //        req.setRawHeader("Range", s.toUtf8());
    //        C_LOG_DEBUG(QString("Range:%1").arg(s));
    //    }
    //    //    req.setHeader(QNetworkRequest::CookieHeader, CBizUtil::getLoginCookies());
    //    m_reply = m_networkAccessMgr->get(req);
    //    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
    //        this, SLOT(onDownloadNetworkError(QNetworkReply::NetworkError)));
    //    connect(m_reply, SIGNAL(finished()), this, SLOT(onHttpFinished()));

    //    if(m_timeoutTimer->isActive())
    //    {
    //        m_timeoutTimer->stop();
    //    }
    //    m_timeoutTimer->start();
}

void CDownloadObject::sendHttpAllGetRequest()
{
    //    C_LOG_FUNCTION;
    //    if(m_status == CPB::TRANSFER_STATE_CANCEL)
    //    {
    //        return;
    //    }

    //    m_downloadInfo.complete = 0;
    //    QUrl url = QUrl(m_url);
    //    QNetworkRequest req(m_url);

    //    m_reply = m_networkAccessMgr->get(req);
    //    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
    //        this, SLOT(onDownloadNetworkError(QNetworkReply::NetworkError)));
    //    connect(m_reply, SIGNAL(finished()), this, SLOT(onHttpFinished()));

    //    if(m_timeoutTimer->isActive())
    //    {
    //        m_timeoutTimer->stop();
    //    }
    //    m_timeoutTimer->start();
}

QString CDownloadObject::getLocalTmpFile()
{
    return m_filePath + m_tmpSuffix;
}

QString CDownloadObject::getSettingsFile()
{
    //    return CLJUtils::getTransportIniFileName(m_filePath);
    //    ;
}

void CDownloadObject::touchFile(const QString &filename)
{
    C_LOG_FUNCTION;
    QFile f(filename);
    f.open(QIODevice::ReadWrite);
    f.close();
}

void CDownloadObject::saveToFile(const QByteArray &dataArray, const QString &fileName, quint64 pos)
{
    C_LOG_FUNCTION;
    C_LOG_DEBUG(QString("dataRead=%1").arg(dataArray.size()));
    {
        //写数据文件
        QFile f(fileName);
        bool isOk = f.open(QIODevice::ReadWrite);
        if(!isOk)
        {
            C_LOG_ERROR(QString("file(%1) open is error").arg(fileName));
            return;
        }
        isOk = f.seek(pos);
        if(!isOk)
        {
            C_LOG_ERROR(QString("file(%1) seek pos (%2) is error").arg(fileName).arg(pos));
            f.close();
            return;
        }
        qint64 writtenSize = f.write(dataArray);
        f.close();
        if(writtenSize <= -1)
        {
            C_LOG_ERROR(QString("file(%1) write is error").arg(fileName));
            return;
        }
        //TODO
        if(writtenSize != dataArray.size())
        {
            C_LOG_WARNING(QString("file(%1) write size is not equal(writtenSize %2,m_dataCache.size %3")
                              .arg(fileName)
                              .arg(writtenSize)
                              .arg(dataArray.size()));
        }

        m_downloadInfo.complete = pos + writtenSize;
        m_downloadInfo.lastTime = QDateTime::currentDateTime();
        m_downloadInfo.save();
        emit sigProcessUpdated(m_downloadInfo.complete, m_downloadInfo.fileSize);
        C_LOG_TEST(QString("writtenSize=%1,m_dataCache=%2").arg(writtenSize).arg(dataArray.size()));
    }
}

void CDownloadObject::requestHead()
{
    setStatus(STATUS_REQUEST_HEAD);
    QUrl url = QUrl(m_url);
    QNetworkRequest req(url);
    req.setRawHeader("Accept-Encoding", "none");
    //    req.setHeader(QNetworkRequest::CookieHeader, CBizUtil::getLoginCookies());
    m_reply = m_networkAccessMgr->head(req);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    //    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onDownloadNetworkError(QNetworkReply::NetworkError)));

    m_timeoutTimer->start();
}

void CDownloadObject::doError(const QString &error)
{
    C_LOG_WARNING(QString("download is error,%1").arg(C_LOG_P(error)));
    m_lastError = error;
    m_timeoutTimer->stop();
    setStatus(STATUS_FINISHED);
}

void CDownloadObject::requestPartial()
{
    if(m_status == STATUS_REQUEST_PARTIAL)
    {
        return;
    }
    if(m_downloadInfo.complete >= m_downloadInfo.fileSize)
    {
        C_LOG_WARNING(QString("Invalid args, complete = %1, filesize = %2").arg(m_downloadInfo.complete).arg(m_downloadInfo.fileSize));
        return;
    }

    QUrl url = QUrl(m_url);
    QNetworkRequest req(m_url);
    C_LOG_DEBUG(QString("complete=%1,filesize=%2,CHUNK_SIZE=%3, url:=%4").arg(m_downloadInfo.complete).arg(m_downloadInfo.fileSize).arg(CHUNK_SIZE).arg(m_url));

    quint64 finishedDownloadSize = m_downloadInfo.complete + m_dataCache.size();
    if(finishedDownloadSize + CHUNK_SIZE > m_downloadInfo.fileSize)
    {
        QString s = QString("bytes=%1-%2").arg(finishedDownloadSize).arg(QString(""));
        req.setRawHeader("Range", s.toUtf8());
        C_LOG_DEBUG(QString("Range:%1").arg(s));
    }
    else
    {
        QString s = QString("bytes=%1-%2").arg(finishedDownloadSize).arg(finishedDownloadSize + CHUNK_SIZE - 1);
        req.setRawHeader("Range", s.toUtf8());
        C_LOG_DEBUG(QString("Range:%1").arg(s));
    }
    //    req.setHeader(QNetworkRequest::CookieHeader, CBizUtil::getLoginCookies());
    m_reply = m_networkAccessMgr->get(req);
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
        this, SLOT(onDownloadNetworkError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(onHttpFinished()));

    if(m_timeoutTimer->isActive())
    {
        m_timeoutTimer->stop();
    }
    m_timeoutTimer->start();
}

void CDownloadObject::onHttpFinished()
{
    C_LOG_FUNCTION;
    //    m_timeoutTimer->stop();

    //    if(m_status == CPB::TRANSFER_STATE_CANCEL)
    //    {
    //        return;
    //    }

    //    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    //    if(m_reply != reply)
    //    {
    //        //该返回的请求已经过期，忽略
    //        return;
    //    }

    //    if(reply->error() != QNetworkReply::NoError)
    //    {
    //        C_LOG_ERROR(QString("error on download, req = %1").arg(reply->request().url().toString() + QString(", reply = ") + reply->readAll()));

    //        QString err = reply->errorString();
    //        C_LOG_ERROR(QString("err no:%1, err string is:%2").arg(reply->error()).arg(err));
    //        setStatus(CPB::TRANSFER_STATE_FAIL);

    //        return;
    //    }
    //    //    QString strErr = reply->rawHeader(QString("X-Starfish-Error").toUtf8());
    //    //    if (strErr.compare("") != 0)
    //    //    {
    //    //        QString logInfo = QString("error on download, req = ") + reply->request().url().toString() +
    //    //                QString(", xerror = ") + strErr + QString(", reply = ") + reply->readAll();
    //    //        C_LOG_ERROR(logInfo);
    //    //        setState(CPB::TRANSFER_STATE_FAIL);
    //    //        return;
    //    //    }
    //    QString strLocalTmpFile = getLocalTmpFile();
    //    ;
    //    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //    C_LOG_DEBUG(QString("statusCode is:%1").arg(statusCode));
    //    QByteArray newTimestamp = reply->rawHeader("Last-Modified");
    //    if(statusCode == OK && m_downloadInfo.fileSize == 0)
    //    {
    //        quint64 contentLength = reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    //        m_downloadInfo.complete = 0;
    //        m_downloadInfo.fileSize = contentLength;
    //        m_downloadInfo.timestamp = newTimestamp;
    //        m_downloadInfo.save();

    //        C_LOG_DEBUG(QString("file size:%1").arg(m_downloadInfo.fileSize));
    //        QByteArray dataRead = reply->readAll();
    //        emit sigProcessUpdated(m_downloadInfo.complete, m_downloadInfo.fileSize);
    //        if(contentLength == 0)
    //        {
    //            // 对于长度为0的文件，直接创建空文件返回
    //            touchFile(m_filePath);
    //            setStatus(CPB::TRANSFER_STATE_SUCCESS);
    //        }
    //        else
    //        {
    //            setStatus(CPB::TRANSFER_STATE_DOING);
    //            sendHttpPartialGetRequest();
    //        }
    //    }
    //    else if(statusCode == PartialContent || m_downloadInfo.fileSize != 0)
    //    {
    //        //检查时间戳，如果和已下载数据的时间戳不一致，重新下载
    //        //if(newTimestamp != m_downloadInfo.timestamp)
    //        //{
    //        //    sendHttpHeadRequest();
    //        //    return;
    //        //}
    //        QByteArray dataRead = reply->readAll();

    //        int cacheSize = m_cacheSize;
    //        C_LOG_OUT_VALUE5(dataRead.size(), m_dataCache.size(), cacheSize, m_downloadInfo.complete, m_downloadInfo.fileSize);

    //        if(dataRead.size() < 512 && statusCode == OK)
    //        {
    //            //在某些网络环境下，断点续传会失败，状态码为200, 此时进行补充处理， 一次性获取全部文件
    //            sendHttpAllGetRequest();
    //            return;
    //        }

    //        //先存到内存里，当>= s_uiCacheSize时，存到文件里
    //        m_dataCache.append(dataRead);

    //        if(m_dataCache.size() >= m_cacheSize || m_downloadInfo.complete + m_dataCache.size() >= m_downloadInfo.fileSize)
    //        {
    //            saveToFile(m_dataCache, strLocalTmpFile, m_downloadInfo.complete);
    //            m_dataCache.clear();
    //        }
    //        if(m_downloadInfo.complete >= m_downloadInfo.fileSize)
    //        {
    //            if(QFile::exists(m_filePath))
    //            {
    //                QFile::remove(m_filePath);
    //            }
    //            QFile f(strLocalTmpFile);
    //            f.rename(m_filePath);
    //            setStatus(CPB::TRANSFER_STATE_SUCCESS);
    //        }
    //        else
    //        {
    //            sendHttpPartialGetRequest();
    //        }
    //    }
    //    else
    //    {
    //        C_LOG_ERROR(QString("Error on download, http status code: %1").arg(statusCode));
    //        setStatus(CPB::TRANSFER_STATE_FAIL);
    //    }
}

void CDownloadObject::onFullDownloadFinished()
{
    //    C_LOG_FUNCTION;
    //    QFile f(m_filePath);
    //    if(!f.open(QIODevice::WriteOnly))
    //        return;

    //    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    //    qint64 writeLen = f.write(reply->readAll());
    //    C_LOG_VALUE(C_LOG_LEVEL_INFO, "download finished", writeLen);
    //    f.close();
    //    delete reply;

    //    setStatus(CPB::TRANSFER_STATE_SUCCESS);
}

void CDownloadObject::onRequestTimeout()
{
    C_LOG_FUNCTION;
    if(m_reply)
    {
        C_LOG_WARNING("File download request timeout.");
        m_reply->close();
    }
}

void CDownloadObject::onDownloadNetworkError(QNetworkReply::NetworkError code)
{
    //    C_LOG_FUNCTION;
    //    C_LOG_ERROR(QString("Download network error, errcode:%1.").arg(code));
    //    m_timeoutTimer->stop();

    //    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    //    if(m_reply != reply)
    //    {
    //        //该返回的请求已经过期，忽略
    //        return;
    //    }

    //    if(code != QNetworkReply::NoError)
    //    {
    //        // TODO 目前的重发策略有问题，先屏掉
    //        //       if(m_curRetryTime < m_maxRetryTime)
    //        //       {
    //        //           //重发一次网络请求
    //        //           m_curRetryTime++;
    //        //           downloadFile(m_fileInfoTransFile.m_fileBindInfo.m_strUrl);
    //        //           return;
    //        //       }
    //        //       else
    //        {
    //            setStatus(CPB::TRANSFER_STATE_FAIL);
    //            C_LOG_ERROR(QString("Download error, url:%1").arg(reply->url().toString()));
    //        }
    //    }
}

void CDownloadObject::onDownloadFile(QString filePath, QString url, bool fullDownload)
{
    downloadFile(filePath, url, fullDownload);
}

void CDownloadObject::onHeadRequestFinished()
{
    //
}

void CDownloadObject::onRequestFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    bool ok = reply->error() == QNetworkReply::NoError;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    switch(m_status)
    {
    case STATUS_REQUEST_HEAD:
    {
        if(ok)
        {
            quint64 contentLength = reply->rawHeader("rawHeader").toULongLong();
            QByteArray newTimestamp = reply->rawHeader("Last-Modified");
            if(contentLength == 0)
            {
                //todo all;
            }
            else
            {
                if(m_downloadInfo.fileSize != contentLength || newTimestamp != m_downloadInfo.timestamp)
                {
                    C_LOG_OUT_V4(m_downloadInfo.fileSize, contentLength, newTimestamp, m_downloadInfo.timestamp);
                    m_downloadInfo.fileSize = contentLength;
                    m_downloadInfo.timestamp = newTimestamp;
                    m_downloadInfo.complete = 0;
                    m_downloadInfo.lastTime = QDateTime::currentDateTime();
                    m_downloadInfo.save();
                }
                setStatus(STATUS_REQUEST_PARTIAL);
            }
        }
        break;
    }
    case STATUS_REQUEST_PARTIAL:
    {
        break;
    }
    default:
        break;
    }
}

CDownloadObject::DownloadInfo::DownloadInfo()
{
    //
}

CDownloadObject::DownloadInfo::DownloadInfo(const QString &fileNamePath)
    : filePath(fileNamePath)
{
    QSettings settings(filePath, QSettings::IniFormat);
    complete = settings.value(QString("complete")).toULongLong();
    fileSize = settings.value(QString("fileSize")).toULongLong();
    lastTime = settings.value(QString("lastTime")).toDateTime();
    timestamp = settings.value(QString("timestamp")).toByteArray();
}

void CDownloadObject::DownloadInfo::save()
{
    QSettings settings(filePath, QSettings::IniFormat);
    settings.setValue(QString("complete"), complete);
    settings.setValue(QString("fileSize"), fileSize);
    settings.setValue(QString("lastTime"), lastTime);
    settings.setValue(QString("timestamp"), timestamp);
    settings.sync();
}
