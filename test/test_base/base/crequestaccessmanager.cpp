#include "crequestaccessmanager.h"
#include "cbaserequestaction.h"
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QThread>
#include <QTimer>

CRequestAccessManager *CRequestAccessManager::m_instance = NULL;
CRequestAccessManager *CRequestAccessManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CRequestAccessManager();
    }
    return m_instance;
}

CRequestAccessManager::CRequestAccessManager(QObject *parent)
    : QObject(parent)
    , m_access(NULL)
{
    m_access = new QNetworkAccessManager();
    connect(m_access, SIGNAL(sslErrors(QNetworkReply *, QList<QSslError>)), this, SLOT(onIgnoreSSlErrors(QNetworkReply *, QList<QSslError>)));
    //    connect(m_access, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestFinished(QNetworkReply*)));
}

void CRequestAccessManager::onIgnoreSSlErrors(QNetworkReply *reply, QList<QSslError> sslErrors)
{
    for(int i = 0; i < sslErrors.size(); ++i)
    {
        C_LOG_ERROR(QString("Get ssl error [%1]").arg(sslErrors[i].errorString()));
    }
    reply->ignoreSslErrors(sslErrors);
}

bool CRequestAccessManager::request(CBaseRequestAction *action)
{
    if(!action)
    {
        return false;
    }
    CBaseRequestAction::Operation operation = action->getOperation();
    C_LOG_OUT_V2(operation, action->metaObject()->className());
    QNetworkReply *reply = NULL;
    switch(operation)
    {
    case CBaseRequestAction::PostByteArray:
        reply = postByteArray(action);
        break;
    case CBaseRequestAction::Get:
        reply = get(action);
        break;
    case CBaseRequestAction::PatchByteArray:
        reply = patchByteArray(action);
        break;
    case CBaseRequestAction::PatchMultimedia:
        reply = patchMulti(action);
        break;
    case CBaseRequestAction::PostWithttachment:
        reply = postWithttachment(action);
        break;
    case CBaseRequestAction::Delete:
        reply = deleteResource(action);
        break;
    case CBaseRequestAction::PutByteArray:
        reply = putByteArray(action);
        break;
    default:
        C_LOG_ERROR(QString("Bad action with type = [%1]").arg(operation));
        break;
    }
    if(reply)
    {
        connect(reply, SIGNAL(finished()), action, SLOT(onReplyFinished()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), action, SLOT(onReplyError(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(sslErrors(const QList<QSslError> &)), action, SLOT(onIgnoreSSlErrors(const QList<QSslError> &)));
        connect(action, SIGNAL(sigCancel()), reply, SLOT(abort()));
        return true;
    }
    return false;
}

QNetworkReply *CRequestAccessManager::postByteArray(CBaseRequestAction *action)
{
    C_LOG_FUNCTION;
    QByteArray array = action->getByteArray();
    QNetworkRequest request = action->getRequest();
    return m_access->post(request, array);
}

QNetworkReply *CRequestAccessManager::putByteArray(CBaseRequestAction *action)
{
    C_LOG_FUNCTION;
    QByteArray array = action->getByteArray();
    QNetworkRequest request = action->getRequest();
    return m_access->put(request, array);
}

QNetworkReply *CRequestAccessManager::postWithttachment(CBaseRequestAction *action)
{
    C_LOG_FUNCTION;
    QNetworkRequest request = action->getRequest();
    QHttpMultiPart *multiPart = action->getMultiPart();
    return m_access->post(request, multiPart);
}

QNetworkReply *CRequestAccessManager::get(CBaseRequestAction *action)
{
    C_LOG_FUNCTION;
    QNetworkRequest request = action->getRequest();
    return m_access->get(request);
}

QNetworkReply *CRequestAccessManager::patchByteArray(CBaseRequestAction *action)
{
    C_LOG_FUNCTION;
    QNetworkRequest request = action->getRequest();
    QByteArray array = action->getByteArray();
    QBuffer *buffer = new QBuffer;
    buffer->setData(array);
    QString patchString = array;
    QNetworkReply *reply = m_access->sendCustomRequest(request, QByteArray("PATCH"), buffer);
    buffer->setParent(reply);
    return reply;
}

QNetworkReply *CRequestAccessManager::patchMulti(CBaseRequestAction *action)
{
    QNetworkRequest request = action->getRequest();
    QByteArray array = action->getByteArray();
    QBuffer *buffer = new QBuffer;
    buffer->setData(array);
    QNetworkReply *reply = m_access->sendCustomRequest(request, QByteArray("PATCH"), buffer);
    buffer->setParent(reply);
    return reply;
}

QNetworkReply *CRequestAccessManager::deleteResource(CBaseRequestAction *action)
{
    QNetworkRequest request = action->getRequest();
    return m_access->deleteResource(request);
}

void CRequestAccessManager::recoverNetworkAccessible()
{
    m_access->setNetworkAccessible(QNetworkAccessManager::NotAccessible);
    m_access->clearAccessCache();
    m_access->setNetworkAccessible(QNetworkAccessManager::Accessible);
}
