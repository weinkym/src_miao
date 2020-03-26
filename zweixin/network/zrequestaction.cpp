#include "zrequestaction.h"
#include <QSize>
#include <QFile>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include "zrequestaccessmanager.h"
#include <QSize>
#include <QFile>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <ctime>
#include <QCryptographicHash>
#include "zrequestaction.h"
#include "zrequestaccessmanager.h"


//QString CRequestAction::m_appKey = "d82680c1ca7e435b98a251472261880c";
//QString CRequestAction::m_secretKey = "d7a5fea84fe93a706edb16473cc89ee94dab3719";
//QString CRequestAction::m_appKey = "d842d7d064dd4338b0632472b5a4ddcb";
QString ZRequestAction::m_appKey = "77d632c3dbd5421bb13bdbacad0da52a";
//QString CRequestAction::m_secretKey = "2178ffb77f74bfa48b137e6f9aa4179af1d11892";

//QString CRequestAction::m_secretKey = "301c46a6e0b621f99282d1fa585f95870d715113";
QString ZRequestAction::m_secretKey = "9ad3c9d917ef370b3b91a52150404202117e1775";

ZRequestAction::ZRequestAction(HttpRequestType type, QObject *parent) :
    QObject(parent),
    m_networkReply(NULL),
    m_httpTimeout(m_defaultHttpTimeout),
    m_maxRetryTime(m_defaultMaxRetryTime),
    m_curRetryTime(0),
    m_msgSendCancelled(false),
    m_timerTimeout(NULL)
{
    m_replayStatusData.type = type;
    m_replayStatusData.uuid = QUuid::createUuid();
    initTimeoutTimer();
}

ZRequestAction::~ZRequestAction()
{
    //    LOG_DEBUG << QString("http request(type=%1) consume: %2ms")
    //                 .arg(m_type).arg(m_timeTest.elapsed());
    if(m_networkReply)
    {
        delete m_networkReply;
        m_networkReply = NULL;
    }
}

void ZRequestAction::trigger()
{
    m_replayStatusData.startTime = QDateTime::currentDateTime();
    Operation operation = getOperation();
    ZRequestAccessManager *accessManager = ZRequestAccessManager::getInstance();
    switch(operation)
    {
    case PostByteArray:
        accessManager->postByteArray(this);
        break;
    case Get:
        accessManager->get(this);
        break;
    case PatchByteArray:
        accessManager->patchByteArray(this);
        break;
    case PatchMultimedia:
        accessManager->patchMulti(this);
        break;
    case PostWithttachment:
        accessManager->postWithttachment(this);
        break;
    case Delete:
        accessManager->deleteResource(this);
        break;
    case PutByteArray:
        accessManager->putByteArray(this);
        break;
    default:
        ZW_LOG_CRITICAL(QString("Bad action with type = [%1]").arg(operation));
        break;
    }
}

// !TODO add by Guilai, 函数名字和trigger 很奇怪，后续调整，数据获取途径可能也可以调整
void ZRequestAction::startRequestAction()
{
    trigger();
}

void ZRequestAction::setReply(QNetworkReply *reply)
{
    if(reply == NULL)
    {
        ZW_LOG_CRITICAL("param reply is NULL");
        return;
    }
    if(m_networkReply)
    {
        disconnect(m_networkReply,SIGNAL(finished()),
                   this,SLOT(onReplyFinished()));
        m_networkReply->deleteLater();
        m_networkReply = NULL;
    }
    m_networkReply = reply;
    connect(m_networkReply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
    connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));
    connect(m_networkReply, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onIgnoreSSlErrors(const QList<QSslError>&)));

    m_timerTimeout->setInterval(m_httpTimeout);
    m_timerTimeout->start();
}

int ZRequestAction::getType() const
{
    return m_replayStatusData.type;
}

void ZRequestAction::setType(HttpRequestType type)
{
    m_replayStatusData.type = type;
}

QString ZRequestAction::createBoundary()
{
    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    QString boundary = QByteArray("boundary_.oOo._")
            + QByteArray::number(qrand()).toBase64()
            + QByteArray::number(qrand()).toBase64()
            + QByteArray::number(qrand()).toBase64();
    return boundary;
}

void ZRequestAction::requestFromHttp()
{
    trigger();
}

QString ZRequestAction::description()
{
    QString dataString(getByteArray());
    return QString("type:%1 url:%2 dataString_simplified: %3 dataString:%4")
            .arg(m_replayStatusData.type).arg(dataString.simplified()).arg(dataString);
}

void ZRequestAction::initTimeoutTimer()
{
    m_timerTimeout = new QTimer(this);
    m_timerTimeout->setSingleShot(true);
    connect(m_timerTimeout, SIGNAL(timeout()), this, SLOT(onHttpTimeout()));
}

ZRequestAction::Operation ZRequestAction::getOperation()
{
    return Undefine;
}

QByteArray ZRequestAction::getByteArray() const
{
    return QByteArray();
}

QHttpMultiPart *ZRequestAction::getMultiPart()
{
    return NULL;
}

void ZRequestAction::onReplyError(QNetworkReply::NetworkError error)
{
    ZW_LOG_CRITICAL(QString("Get reply error no = [%1]").arg(error));
    if (error == QNetworkReply::UnknownNetworkError)
    {
        ZRequestAccessManager::getInstance()->recoverNetworkAccessible();
    }

    if(m_networkReply == NULL)
    {
        ZW_LOG_CRITICAL(QString("Bad network reply."));
        return;
    }
}

void ZRequestAction::onHttpTimeout()
{
    ZW_LOG_FUNCTION;
    if(m_networkReply == NULL)
    {
        ZW_LOG_CRITICAL(QString("Http timeout, bad network reply"));
        return;
    }

    // !TODO add by Guilai, should delete the resource here.
    //ZRequestAccessManager::getInstance()->deleteResource(m_request);
    m_networkReply->abort();
    // !TODO add by Guilai, should call deleteLater or call deleteLater in onReplyFinished.
    //m_networkReply->deleteLater();
}

void ZRequestAction::onIgnoreSSlErrors(QList<QSslError> sslErrors)
{
    for (int i = 0; i < sslErrors.size(); ++i)
    {
        ZW_LOG_CRITICAL(QString("Get ssl error [%1]").arg(sslErrors[i].errorString()));
    }

    m_networkReply->ignoreSslErrors(sslErrors);
}

void ZRequestAction::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply == NULL)
    {
        ZW_LOG_CRITICAL(QString("Get bad QNetworkReply = NLL"));
        return;
    }

    if(m_networkReply != reply)
    {
        //该网络请求已过期
        ZW_LOG_WARNING(QString("This network request is overdue, action type:%1").arg(m_replayStatusData.type));
        return;
    }
    m_timerTimeout->stop();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    ZW_LOG_DEBUG(QString("statusCode=%1").arg(statusCode));
    m_replayStatusData.statusCode = statusCode;

    m_replayStatusData.endTime = QDateTime::currentDateTime();
    m_replayStatusData.networkErrorCode = reply->error();
    int consuming = m_replayStatusData.endTime.time().msecsTo(m_replayStatusData.startTime.time());
    consuming = qAbs(consuming);
    QString urlString = reply->request().url().toString();
    QString logInfo = QString("[HttpReturn] request url:%1  type(%2) consuming %3ms statusCode = %4 getByteArray = %5")
            .arg(urlString).arg(m_replayStatusData.type).arg(consuming).arg(statusCode).arg(QString(this->getByteArray()));

    ZW_LOG_INFO(logInfo);
    m_replayStatusData.replyData = reply->readAll();
    ZW_LOG_DEBUG(QString("[HttpReturn]request return data = %1").arg(QString(m_replayStatusData.replyData)));

    if(m_replayStatusData.networkErrorCode == QNetworkReply::NoError)
    {
        emit sigRequestFinished(m_replayStatusData);
    }
    else
    {
        ZW_LOG_CRITICAL(QString("type:%1,Network error:%2,url:%3").arg(m_replayStatusData.type).arg(m_replayStatusData.networkErrorCode).arg(urlString));
        if(m_curRetryTime < m_maxRetryTime)
        {
            ZW_LOG_WARNING(QString("Action trigger again, m_curRetryTime:%1,action type:%2").
                        arg(m_curRetryTime).arg(m_replayStatusData.type));
            m_curRetryTime++;
            trigger();
            return;
        }
        else
        {
            emit sigRequestFinished(m_replayStatusData);
        }
    }
    this->deleteLater();
}

//QNetworkRequest ZRequestAction::createRequest(const QString &lastHeader, bool needVerify) const
//{
//    LOG_FUNCTION;
//    QString urlPrefix = "https://api-zhibo.yxt.com/v1";

//    QUrl url(urlPrefix + lastHeader);

//    QNetworkRequest request(url);
//    request.setRawHeader("Accept", "application/json");
//    request.setRawHeader("Content-Type", "application/json");
//    request.setRawHeader("Source", "907");
//    return request;
//}

int ZRequestAction::parseErrorCode(const QVariantMap &result)
{
    ZW_LOG_FUNCTION;
    int errorCode = 0;
    if(result.contains("error"))
    {
        QVariantMap errMap = result["error"].toMap();
        if(errMap.contains("key"))
        {
            errorCode = errMap["key"].toInt();
        }
        else if(errMap.contains("code"))
        {
            errorCode = errMap["code"].toInt();
        }
        else
        {
            ZW_LOG_CRITICAL(QString("Bad result, set error code to  -999"));
            errorCode = -999;
        }
    }

    return errorCode;
}

QString ZRequestAction::parseErrorMsg(const QVariantMap &result)
{
    QString errMsg;

    if(result.contains("message"))
    {
        errMsg = result["message"].toString();
    }

    return errMsg;
}

