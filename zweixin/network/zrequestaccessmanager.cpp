#include "zrequestaccessmanager.h"
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QThread>
#include <QTimer>

ZRequestAccessManager *ZRequestAccessManager::m_instance = NULL;
ZRequestAccessManager *ZRequestAccessManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new ZRequestAccessManager();
    }
    return m_instance;
}

ZRequestAccessManager::ZRequestAccessManager(QObject *parent) :
    QObject(parent),
    m_access(NULL),
    m_isYxtLogining(false)
{
    ZW_LOG_FUNCTION;
    m_access = new QNetworkAccessManager();
    connect(m_access, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(onIgnoreSSlErrors(QNetworkReply*,QList<QSslError>)));
    connect(m_access, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
            this, SLOT(onNetworkAccessibilityChanged(QNetworkAccessManager::NetworkAccessibility)));
    connect(m_access, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestFinished(QNetworkReply*)));
}

// !TODO add by Guilai; 函数可能返回错误，上层逻辑需要处理这个错误信息，不能返回void
bool ZRequestAccessManager::setCookie(const QVariant &cookieListVariant, const QString &domain)
{
    bool ret = false;
    QList<QNetworkCookie> cookieList = cookieListVariant.value<QList<QNetworkCookie> >();
    if(!cookieList.isEmpty())
    {
        QNetworkCookieJar *cookieJar = m_access->cookieJar();
        if(cookieJar != NULL)
        {
            foreach(QNetworkCookie cookie, cookieList)
            {
                cookie.setDomain(domain);
                if (cookieJar->updateCookie(cookie) == false)
                {
                    ZW_LOG_CRITICAL(QString("update cookie failed. The cookie is not effect."));
                    ret = false;
                    break;
                }
            }
            ret = true;
        }
        else
        {
            cookieJar = new QNetworkCookieJar();
            foreach(QNetworkCookie cookie, cookieList)
            {
                cookie.setDomain(domain);
                if (cookieJar->insertCookie(cookie) == false)
                {
                    ZW_LOG_CRITICAL(QString("Insert cookie failed. The cookie is not effect."));
                    ret = false;
                    break;
                }
            }

            m_access->setCookieJar(cookieJar);
            ret = true;
        }
    }
    else
    {
        ZW_LOG_CRITICAL(QString("Set bad cookie list. Cookie list are empty."));
        ret = false;
    }

    return ret;
}

// !TODO add by Guilai; 函数可能返回错误，上层逻辑需要处理这个错误信息，不能返回void
void ZRequestAccessManager::refreshYxtToken()
{
    //如果正在发起登录，忽略
    if(m_isYxtLogining)
    {
        ZW_LOG_CRITICAL(QString("Login is processing, call login too much times"));
        return;
    }
    else
    {
        //通过重登录来刷新本地cookie(yxt-token)
        m_isYxtLogining = true;
        emit sigYxtTokenChanged();
    }
}

// !TODO add by Guilai; 函数可能返回错误，上层逻辑需要处理这个错误信息，不能返回void
void ZRequestAccessManager::setYxtLoginStatus(bool isLogging)
{
    m_isYxtLogining = isLogging;
}

void ZRequestAccessManager::onRequestFinished(QNetworkReply *reply)
{
    Q_UNUSED(reply);
}

void ZRequestAccessManager::onNetworkAccessibilityChanged(QNetworkAccessManager::NetworkAccessibility status)
{
    bool networkAccessibility = false;
    if (status == QNetworkAccessManager::UnknownAccessibility)
    {
        ZW_LOG_WARNING(QString("Unknown network accessibility."));
    }
    else if (status == QNetworkAccessManager::NotAccessible)
    {
        ZW_LOG_WARNING(QString("network is not accessible"));
    }
    else if (status == QNetworkAccessManager::Accessible)
    {
        networkAccessibility = true;
        ZW_LOG_WARNING(QString("network is recovered"));
    }
    else
    {
        ZW_LOG_CRITICAL(QString("Got a bad network status, look out."));
    }

    emit sigUpdateNetworkAccessibility(networkAccessibility);
}

void ZRequestAccessManager::onIgnoreSSlErrors(QNetworkReply* reply, QList<QSslError> sslErrors)
{
    for (int i = 0; i < sslErrors.size(); ++i)
    {
        ZW_LOG_CRITICAL(QString("Get ssl error [%1]").arg(sslErrors[i].errorString()));
    }
    reply->ignoreSslErrors(sslErrors);
}

void ZRequestAccessManager::postByteArray(ZRequestAction *action)
{
    ZW_LOG_FUNCTION;
    if(action == NULL)
    {
        ZW_LOG_WARNING(QString("action is null"));
        return;
    }
    QByteArray array = action->getByteArray();
    QNetworkRequest request = action->createRequest();

    QString logInfo = QString("[Http Request][POST]url: ") + request.url().toString();
    ZW_LOG_INFO(logInfo);
    QNetworkReply *reply = m_access->post(request, array);
    action->setReply(reply);
}

void ZRequestAccessManager::putByteArray(ZRequestAction *action)
{
    ZW_LOG_FUNCTION;
    if(action == NULL)
    {
        ZW_LOG_WARNING(QString("action is null"));
        return;
    }
    QByteArray array = action->getByteArray();
    QNetworkRequest request = action->createRequest();

    QString logInfo = QString("[Http Request][PUT]url: ") + request.url().toString();
    ZW_LOG_INFO(logInfo);
    QNetworkReply *reply = m_access->put(request, array);
    action->setReply(reply);
}

void ZRequestAccessManager::postWithttachment(ZRequestAction *action)
{
    ZW_LOG_FUNCTION;
    if(action == NULL)
    {
        ZW_LOG_WARNING(QString("action is null"));
        return;
    }
    QNetworkRequest request = action->createRequest();
    QHttpMultiPart *multiPart = action->getMultiPart();

    ZW_LOG_INFO(QString("[Http Request][Post]post url: %1").arg(request.url().toString()));
    QNetworkReply *reply = m_access->post(request,multiPart);

    action->setReply(reply);
}

void ZRequestAccessManager::get(ZRequestAction *action)
{
    ZW_LOG_FUNCTION;
    if(action == NULL)
    {
        ZW_LOG_WARNING(QString("action is null"));
        return;
    }
    QNetworkRequest request = action->createRequest();

    QString url = request.url().toString();
    if(!url.startsWith("http"))
    {
        QString typeString = QString::number(action->getType());
        ZW_LOG_CRITICAL(QString("[Http Request][Get]url not start with http, url is:%1,action type:%2,className:%3")
                  .arg(url).arg(typeString).arg(action->metaObject()->className()));
        return;
    }
    ZW_LOG_INFO(QString("[Http Request][Get]get url: %1").arg(request.url().toString()));
    QNetworkReply *reply = m_access->get(request);
    action->setReply(reply);
}

void ZRequestAccessManager::patchByteArray(ZRequestAction *action)
{
    ZW_LOG_FUNCTION;
    if(action == NULL)
    {
        ZW_LOG_WARNING(QString("action is null"));
        return;
    }
    QNetworkRequest request = action->createRequest();
    QByteArray array = action->getByteArray();

    QBuffer *buffer = new QBuffer;
    buffer->setData(array);

    QString patchString = array;
    ZW_LOG_DEBUG(QString("[Http Request][Patch]array:%1  url:%2").arg(patchString).arg( request.url().toString()));
    QNetworkReply *reply = m_access->sendCustomRequest(request, QByteArray("PATCH"), buffer);
    action->setReply(reply);
    buffer->setParent(reply);
}

void ZRequestAccessManager::patchMulti(ZRequestAction *action)
{
    if(action == NULL)
    {
        ZW_LOG_WARNING(QString("action is null"));
        return;
    }
    QNetworkRequest request = action->createRequest();
    QByteArray array = action->getByteArray();

    QBuffer *buffer = new QBuffer;
    buffer->setData(array);

    ZW_LOG_INFO(QString("[Http Request][Patch]patchMulti url: ") + request.url().toString());
    QNetworkReply *reply = m_access->sendCustomRequest(request, QByteArray("PATCH"), buffer);
    action->setReply(reply);
    buffer->setParent(reply);
}

void ZRequestAccessManager::deleteResource(ZRequestAction *action)
{
    if(action == NULL)
    {
        ZW_LOG_CRITICAL(QString("action is null"));
        return;
    }
    QNetworkRequest request = action->createRequest();
    ZW_LOG_INFO(QString("[Http Request][Delete]url: ") + request.url().toString());
    QNetworkReply *reply = m_access->deleteResource(request);
    action->setReply(reply);
}

// !TODO add by Guilai. QNetworkAccessManager have bug, so call the function when get network error = 99;
void ZRequestAccessManager::recoverNetworkAccessible()
{
    m_access->setNetworkAccessible(QNetworkAccessManager::NotAccessible);
    m_access->clearAccessCache();
    m_access->setNetworkAccessible(QNetworkAccessManager::Accessible);
}

bool ZRequestAccessManager::getNetworkAccessible()
{
    if (m_access->networkAccessible() == QNetworkAccessManager::Accessible)
    {
        return true;
    }
    else
    {
        return false;
    }
}
