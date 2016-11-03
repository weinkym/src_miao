#include "zrequestaction.h"
#include <QSize>
#include <QFile>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include "zrequestaccessmanager.h"

ZRequestAction::ZRequestAction(HttpRequestType type, QObject *parent) :
    QObject(parent),
    m_networkReply(NULL),
    m_nReplyErrCode(QNetworkReply::NetworkError::NoError),
    m_httpTimeout(m_defaultHttpTimeout),
    m_maxRetryTime(m_defaultMaxRetryTime),
    m_curRetryTime(0),
    m_type(type),
    m_timerTimeout(NULL)
{
    m_timeTest.start();
    m_uuid = QUuid::createUuid();
    initTimeoutTimer();
}

ZRequestAction::~ZRequestAction()
{
    if(m_networkReply)
    {
        delete m_networkReply;
        m_networkReply = NULL;
    }
}

void ZRequestAction::trigger()
{
    Operation operation = getOperation();
    LOG_DEBUG(QString("trigger action with type = [%1]").arg(operation));
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
        LOG_DEBUG(QString("Bad action with type = [%1]").arg(operation));
        break;
    }
}

void ZRequestAction::setReply(QNetworkReply *reply)
{
    LOG_FUNCTION;
    if(reply == NULL)
    {
        LOG_WARNING(QString("param reply is NULL"));
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
    LOG_TEST(QString("reply isFinished %1").arg(reply->isFinished() ? "true":"false"));
    connect(m_networkReply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
    connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));
    connect(m_networkReply, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onIgnoreSSlErrors(const QList<QSslError>&)));

    m_timerTimeout->setInterval(m_httpTimeout);
    m_timerTimeout->start();
}

HttpRequestType ZRequestAction::getType() const
{
    return m_type;
}

void ZRequestAction::setType(HttpRequestType type)
{
    m_type = type;
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


void ZRequestAction::requestFromDB()
{
    //TODO 子类自身实现从数据库获取数据逻辑
}

void ZRequestAction::requestFromHttp()
{
    trigger();
}

void ZRequestAction::sendFinishedSignal(const quint64 errorCode, bool fromSever, const QByteArray &value, int type)
{
   ZRequestResponse response;
   response.code = errorCode;
   response.fromSever = fromSever;
   response.data = value;
   response.type = type;
    emit sigFinished(response);
}

QString ZRequestAction::description()
{
    QString dataString(getByteArray());
    return QString("type:%1 url:%2 dataString_simplified: %3 dataString:%4")
            .arg(m_type).arg(dataString.simplified()).arg(dataString);
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

QNetworkRequest ZRequestAction::getRequest() const
{
    return QNetworkRequest();
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
    LOG_ERROR(QString("Get reply error no = [%1]").arg(error));
    if (error == QNetworkReply::UnknownNetworkError)
    {
        ZRequestAccessManager::getInstance()->recoverNetworkAccessible();
    }

    if(m_networkReply == NULL)
    {
        LOG_ERROR(QString("Bad network reply."));
        return;
    }

    // !TODO add by Guilai, should delete the resource here.
    //ZRequestAccessManager::getInstance()->deleteResource(m_request);
    //m_networkReply->abort();
    // !TODO add by Guilai, should call deleteLater or call deleteLater in onReplyFinished.
    //m_networkReply->deleteLater();
}

void ZRequestAction::onHttpTimeout()
{
    LOG_FUNCTION;
    if(m_networkReply == NULL)
    {
        LOG_ERROR(QString("Http timeout, bad network reply"));
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
        LOG_ERROR(QString("Get ssl error [%1]").arg(sslErrors[i].errorString()));
    }

    m_networkReply->ignoreSslErrors(sslErrors);
}

void ZRequestAction::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply == NULL)
    {
        LOG_ERROR(QString("Get bad QNetworkReply = NLL"));
        return;
    }

    if(m_networkReply != reply)
    {
        //该网络请求已过期
        LOG_WARNING(QString("This network request is overdue, action type:%1").arg(m_type));
        return;
    }

    int consuming = m_timeTest.msecsTo(QTime::currentTime());
    consuming = qAbs(consuming);
    m_timerTimeout->stop();
    m_nReplyErrCode = reply->error();
    QString urlString = reply->request().url().toString();
    QString logInfo = QString("[Http Return] request url:%1  type(%2) consuming %3ms")
            .arg(urlString)
            .arg(m_type)
            .arg(consuming);
    LOG_INFO(logInfo);
    QByteArray array = reply->readAll();

    LOG_TEST(QString("className:%1--type:%2--consuming:%3--url:%4--data:%5--postData:%6")
             .arg(this->metaObject()->className())
             .arg(m_type)
             .arg(consuming).arg(urlString)
             .arg( QString(array)).arg(QString(getByteArray())));
    LOG_TEST(QString("postData=%1").arg(QString(getByteArray())));

    if(m_nReplyErrCode == QNetworkReply::NoError)
    {
        QTime testTime = QTime::currentTime();
        setReplyData(array, reply);
        LOG_INFO(QString("setReplyData type:%1 consuming:%2")
                 .arg(m_type)
                 .arg(testTime.msecsTo(QTime::currentTime())));
    }

    else
    {
        LOG_ERROR(QString("type:%1,Network error:%2,url:%3").arg(m_type).arg(m_nReplyErrCode).arg(urlString));
        if(m_curRetryTime < m_maxRetryTime)
        {
            LOG_WARNING(QString("Action trigger again, m_curRetryTime:%1,action type:%2").
                        arg(m_curRetryTime).arg(m_type));
            trigger();
            m_curRetryTime++;
            return;
        }
        sendFinishedSignal(reply->error(),false,"",m_type);
    }
}

void ZRequestAction::setReplyData(const QByteArray &dataArray, const QNetworkReply *reply)
{
    Q_UNUSED(dataArray)
    Q_UNUSED(reply)
    sendFinishedSignal(reply->error(),false,dataArray,m_type);
}

QNetworkRequest ZRequestAction::createRequest(const QString &lastHeader, bool isOrgApi, const QString &typeHeader) const
{
    LOG_FUNCTION;
//    QString urlPrefix;
//    if(!isOrgApi)
//        urlPrefix = AppUrl::getHttpUrlPrefix();
//    else
//        urlPrefix = AppUrl::getOrgApiUrl(m_orgId);
//    QUrl url(urlPrefix + lastHeader);
        QUrl url("http://123.57.59.182:9876/v1" + lastHeader);
    QNetworkRequest request(url);
//    //    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//    request.setHeader(QNetworkRequest::ContentTypeHeader, typeHeader);

//    // 这块代码为服务器调试做准备，保证服务器确定命令发送端的操作系统
//    request.setRawHeader("User-Agent", CDbCacheUtils::getAgent().toLatin1());

//    if(isOrgApi)
//    {
//        request.setHeader(QNetworkRequest::CookieHeader, CBizUtil::getLoginCookies());
//    }

    return request;
}

QNetworkRequest ZRequestAction::createAttachmentRequest(const QString &lastHeader, const QString &boundary)
{
    LOG_FUNCTION;
    QString urlPrefix/* = AppUrl::getHttpUrlPrefix()*/;
    QUrl url(urlPrefix + lastHeader);
    QNetworkRequest request(url);
//    QString header = QString("multipart/form-data;%1").arg(boundary);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, header);
    return request;
}

int ZRequestAction::parseErrorCode(const QVariantMap &result)
{
    LOG_FUNCTION;
//    if(result.contains(JK_ERROR))
//    {
//        QVariantMap errMap = result[JK_ERROR].toMap();
//        if(errMap.contains(JK_CODE))
//        {
//            m_nReplyErrCode = errMap[JK_CODE].toInt();
//            if(m_nReplyErrCode == 103)
//            {
//                LOG_ERROR(QString("Yxt token invalid: %1").arg(m_nReplyErrCode));
//                ZRequestAccessManager::getInstance()->refreshYxtToken();
//            }
//        }
//        else
//        {
//            LOG_ERROR(QString("Bad result, set error code to  -999"));
//            m_nReplyErrCode = -999;
//        }
//    }

    return m_nReplyErrCode;
}

QString ZRequestAction::parseErrorMsg(const QVariantMap &result)
{
    QString errMsg;

//    if(result.contains(JK_MSG))
//    {
//        errMsg = result[JK_MSG].toString();
//    }

    return errMsg;
}

QString ZRequestAction::getAttachContentType(const QString &suffix)
{
    LOG_FUNCTION;
    if(suffix == "jpg" || suffix == "jpeg")
        return "image/jpeg";
    else if(suffix == "png")
        return "image/png";
    else if(suffix == "mp4")
        return "audio/mp4";
    return QString();
}
