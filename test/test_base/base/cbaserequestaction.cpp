#include "cbaserequestaction.h"
#include "clogsetting.h"
#include "crequestaccessmanager.h"
#include <QCryptographicHash>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSize>
#include <QTimer>
#include <ctime>

CBaseRequestAction::CBaseRequestAction(CBaseRequestAction::Type type, QObject *parent)
    : QObject(parent)
    , m_networkReply(NULL)
    , m_timerTimeout(NULL)
    , m_httpTimeout(m_defaultHttpTimeout)
    , m_maxRetryTime(m_defaultMaxRetryTime)
    , m_curRetryTime(0)
{
    m_replayStatusData.type = type;
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
        QString error = QString("start action is error,className=%1,type=%2").arg(this->metaObject()->className()).arg(getType());
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

CBaseRequestAction::Type CBaseRequestAction::getType() const
{
    return m_replayStatusData.type;
}

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

    m_replayStatusData.endTime = QDateTime::currentDateTime();
    m_replayStatusData.networkErrorCode = reply->error();
    int consuming = m_replayStatusData.endTime.time().msecsTo(m_replayStatusData.startTime.time());
    consuming = qAbs(consuming);
    QString urlString = reply->request().url().toString();

    QString postArray(this->getByteArray());
    QByteArray returnArray = reply->readAll();
    m_replayStatusData.replyArray = returnArray;

    Operation operation = getOperation();
    QStringList logInfos;
    int api_type = m_replayStatusData.type;
    {
        int networkErrorCode = m_replayStatusData.networkErrorCode;
        int statusCode = m_replayStatusData.statusCode;
        QVariant errorValue = m_replayStatusData.errorValue;
        QVariant customValue = m_replayStatusData.customValue;
        QUuid uuid = m_replayStatusData.uuid;
        //        int state = m_replayStatusData.state;
        QDateTime startTime = m_replayStatusData.startTime;
        QDateTime endTime = m_replayStatusData.endTime;
        //        logInfos.append(C_LOG_P6(api_type,statusCode,consuming,urlString,errorValue,networkErrorCode));
        //        logInfos.append(C_LOG_P6(customValue,startTime,endTime,uuid,operation,postArray));
    }

    int maxLen = 2048;
    if(returnArray.length() > maxLen)
    {
        int index = 0;
        while(index < returnArray.length())
        {
            QByteArray byteArray = returnArray.mid(index, maxLen);
            int len = byteArray.length();
            //C_LOG_OUT_V4(api_type,index,len,byteArray);
            index += maxLen;
        }
    }
    else
    {
        logInfos.append(C_LOG_P(returnArray));
    }

    QString logInfo = logInfos.join(",").simplified();

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
    : type(TYPE_NODEFINED)
    , networkErrorCode(0)
    , statusCode(0)
    , uuid(QUuid::createUuid())
    , startTime(QDateTime::currentDateTime())
    , endTime(QDateTime::currentDateTime())
{
}
