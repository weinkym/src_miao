#include "cbaserequestaction.h"
#include "clogsetting.h"
#include "crequestaccessmanager.h"
#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPair>
#include <QSize>
#include <QTimer>
#include <ctime>

CBaseRequestAction::CBaseRequestAction(QObject *parent)
    : QObject(parent)
    , m_networkReply(NULL)
    , m_timerTimeout(NULL)
    , m_httpTimeout(m_defaultHttpTimeout)
    , m_maxRetryTime(m_defaultMaxRetryTime)
    , m_curRetryTime(0)
{
    //    m_replayStatusData.actionName = type;
    m_replayStatusData.uuid = QUuid::createUuid();
    initTimeoutTimer();
}

CBaseRequestAction::~CBaseRequestAction()
{
    if(m_networkReply)
    {
        delete m_networkReply;
        m_networkReply = NULL;
    }
}

void CBaseRequestAction::startRequestAction()
{
    m_replayStatusData.startTime = QDateTime::currentDateTime();
    bool res = CRequestAccessManager::getInstance()->request(this);
    if(!res)
    {
        QString error = QString("start action is error,className=%1,getActionName=%2").arg(this->metaObject()->className()).arg(getActionName());
        C_LOG_WARNING(error);
    }
}

void CBaseRequestAction::cancel()
{
    //C_LOG_FUNCTION;
    m_timerTimeout->stop();
    m_curRetryTime = m_maxRetryTime;
    emit sigCancel();
}

//CBaseRequestAction::RequestType CBaseRequestAction::getType() const
//{
//    return m_replayStatusData.actionName;
//}

void CBaseRequestAction::initTimeoutTimer()
{
    m_timerTimeout = new QTimer(this);
    m_timerTimeout->setSingleShot(true);
    connect(m_timerTimeout, SIGNAL(timeout()), this, SLOT(onHttpTimeout()));
}

QByteArray CBaseRequestAction::getByteArray() const
{
    return QByteArray();
}

QHttpMultiPart *CBaseRequestAction::getMultiPart()
{
    return NULL;
}

void CBaseRequestAction::onHttpTimeout()
{
    //C_LOG_FUNCTION;
    cancel();
}

void CBaseRequestAction::onCancel(const QUuid &uuid)
{
    if(getUuid() != uuid)
    {
        return;
    }
    this->cancel();
}

void CBaseRequestAction::onReplyError(QNetworkReply::NetworkError error)
{
    C_LOG_ERROR(QString("Get reply error no = [%1]").arg(error));
    //    if (error == QNetworkReply::UnknownNetworkError)
    //    {
    //        CBaseRequestAccessManager::getInstance()->recoverNetworkAccessible();
    //    }
}

void CBaseRequestAction::onIgnoreSSlErrors(const QList<QSslError> &sslErrors)
{
    for(int i = 0; i < sslErrors.size(); ++i)
    {
        C_LOG_ERROR(QString("Get ssl error [%1]").arg(sslErrors[i].errorString()));
    }
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply)
    {
        reply->ignoreSslErrors(sslErrors);
    }
}

void CBaseRequestAction::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if(reply == NULL)
    {
        C_LOG_ERROR(QString("Get bad QNetworkReply = NLL"));
        return;
    }
    m_timerTimeout->stop();
    doPreviousReplyFinished();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    m_replayStatusData.statusCode = statusCode;
    {
        QList<QByteArray> objList = reply->rawHeaderList();

        for(int i = 0; i < objList.count(); ++i)
        {
            QByteArray key = objList.at(i);
            QByteArray value = reply->rawHeader(key);
            C_LOG_OUT_V2(key, value);
            m_replayStatusData.rawHeaderMap.insert(key, value);
        }
    }

    m_replayStatusData.endTime = QDateTime::currentDateTime();
    m_replayStatusData.networkErrorCode = reply->error();
    int consuming = m_replayStatusData.endTime.time().msecsTo(m_replayStatusData.startTime.time());
    consuming = qAbs(consuming);
    QString urlString = reply->request().url().toString();

    QString postArray(this->getByteArray());
    QByteArray returnArray = reply->readAll();
    //    QByteArray returnArray;
    m_replayStatusData.replyArray = returnArray;

    RequestType operation = getRequestType();
    QStringList logInfos;
    QString actionName = m_replayStatusData.actionName;
    {
        int networkErrorCode = m_replayStatusData.networkErrorCode;
        int statusCode = m_replayStatusData.statusCode;
        QVariant errorValue = m_replayStatusData.errorValue;
        QVariant customValue = m_replayStatusData.customValue;
        QUuid uuid = m_replayStatusData.uuid;
        //        int state = m_replayStatusData.state;
        QDateTime startTime = m_replayStatusData.startTime;
        QDateTime endTime = m_replayStatusData.endTime;
        logInfos.append(C_LOG_P6(actionName, statusCode, consuming, urlString, errorValue, networkErrorCode));
        logInfos.append(C_LOG_P6(customValue, startTime, endTime, uuid, operation, postArray));
    }

    int maxLen = 2048;
    //    if(returnArray.length() > maxLen)
    //    {
    //        int index = 0;
    //        while(index < returnArray.length())
    //        {
    //            QByteArray byteArray = returnArray.mid(index, maxLen);
    //            int len = byteArray.length();
    //            C_LOG_OUT_V4(actionName, index, len, byteArray);
    //            index += maxLen;
    //        }
    //    }
    //    else
    //    {
    //        logInfos.append(C_LOG_P(returnArray));
    //    }

    QString logInfo = logInfos.join(",").simplified();
    C_LOG_INFO(logInfo);

    if(m_replayStatusData.networkErrorCode == QNetworkReply::NoError)
    {
        //C_LOG_INFO_TYPE(C_LOG::TYPE_API_DATA,logInfo);
        emit sigRequestFinished(m_replayStatusData);
    }
    else
    {
        //C_LOG_INFO_TYPE(C_LOG::TYPE_API_ERROR_DATA,logInfo);
        if(m_curRetryTime < m_maxRetryTime)
        {
            //C_LOG_WARNING_TYPE(C_LOG::TYPE_API_ERROR_DATA,C_LOG_P2(m_replayStatusData.type,m_curRetryTime));
            m_curRetryTime++;
            QTimer::singleShot(100, this, [this] {
                this->startRequestAction();
            });
            return;
        }
        else
        {
            emit sigRequestFinished(m_replayStatusData);
        }
    }
    this->deleteLater();
}

void CBaseRequestAction::doPreviousReplyFinished()
{
}

CBaseRequestAction::Data::Data()
    : networkErrorCode(0)
    , statusCode(0)
    , uuid(QUuid::createUuid())
    , startTime(QDateTime::currentDateTime())
    , endTime(QDateTime::currentDateTime())
{
}
