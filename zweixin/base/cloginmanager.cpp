#include "cloginmanager.h"
#include "zpublicaction.h"
#include <QJsonDocument>
#include <QDomDocument>
#include "ccontactmanager.h"

CLoginManager * CLoginManager::m_instance = NULL;

CLoginManager *CLoginManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CLoginManager;
    }
    return m_instance;
}

CLoginManager::~CLoginManager()
{

}

void CLoginManager::start()
{
    if(m_isStarting)
    {
        return;
    }
    m_isStarting = true;
    requestUuid();
}


void CLoginManager::requestUuid()
{
    connectAction(ZPublicAction::createLoginUuidAction());
}

void CLoginManager::requestQrCode()
{
    connectAction(ZPublicAction::createQrCodeAction(m_uuid));
}

void CLoginManager::requestCookie()
{
    connectAction(ZPublicAction::createCookieAction(m_uuid,m_ticket,m_scan));
}

void CLoginManager::requestInit()
{
    connectAction(ZPublicAction::createWxInitAction(m_baseRequestParam));
}

void CLoginManager::requestStatusNotify()
{
    connectAction(ZPublicAction::createStatusNotify(m_baseRequestParam,m_userData.UserName,m_userData.UserName));
}

void CLoginManager::onRequestWaitLogin()
{
    connectAction(ZPublicAction::createWaitLoginAction(m_uuid,m_tip));
}

CLoginManager::CLoginManager(QObject *parent)
    :CBaseObject(parent)
{
    m_isStarting = false;
    m_tip = 1;
    m_status = CPB::LOGIN_STATUS_LOGINING;
    m_timer.setInterval(1000);
    m_timer.setSingleShot(true);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(onRequestWaitLogin()));
}

QString CLoginManager::parseUuid(const QByteArray &byteArray)
{
    LOG_FUNCTION;
    QString uuid;
    QString tempData = QString(byteArray).simplified();
    LOG_TEST(QString("TTTTTTTV:tempData=%1").arg(tempData));
    QString key = "code=200";
//    if(tempData.contains(key))
    {
        int index1 = tempData.indexOf("\"");
        int index2 = tempData.lastIndexOf("\"");
        if(index1 != index2 && index1 >= 0 && index2 >= 0)
        {
            uuid = tempData.mid(index1+1,index2-index1-1);
        }
        LOG_TEST(QString("TTTTTTTV:index1=%1").arg(index1));
        LOG_TEST(QString("TTTTTTTV:index2=%1").arg(index2));

//        QString tempStr =
    }
    LOG_TEST(QString("TTTTTTTV:uuid=%1").arg(uuid));
    return uuid;
}

bool CLoginManager::parseCookieData(const QByteArray &byteArray, Z_WX_COOKIE_PARAM &param)
{
    QDomDocument doc;
    QString errorString;
    bool ok = doc.setContent(byteArray,&errorString);
    if(!ok)
    {
        LOG_ERROR(errorString);
        return false;
    }
    QDomElement root = doc.documentElement();
    QDomNodeList domNodeList = root.childNodes();
    int count = domNodeList.count();

    for(int i = 0; i < count; ++i)
    {
        QDomNode domNode = domNodeList.at(i);
        QDomElement element = domNode.toElement();
        LOG_TEST(QString("nodeName=%1,%2,%3").arg(element.tagName()).arg(element.nodeName()).arg(element.text()));
        if(element.tagName() == "isgrayscale")
        {
            param.isgrayscale = element.text().toInt();
        }
        else if(element.tagName() == "ret")
        {
            param.ret = element.text().toInt();
        }
        else if(element.tagName() == "skey")
        {
            param.skey = element.text();
        }
        else if(element.tagName() == "wxsid")
        {
            param.wxsid = element.text();
        }
        else if(element.tagName() == "wxuin")
        {
            param.wxuin = element.text();
        }
        else if(element.tagName() == "pass_ticket")
        {
            param.pass_ticket = element.text();
        }
        else
        {
//            ok = lse;
        }
    }
    return ok;
}

bool CLoginManager::parseRedirectUri(const QByteArray &byteArray)
{
    //(window.redirect_uri)=\"([\w\-\.,@?^=%&amp;:/~\+#]*[\w\-\@?^=%&amp;/~\+#])?
    QList<QStringList> redirecttUrlData = Zpublic::regexCapture(QString(byteArray),"(window.redirect_uri)=\\\"([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?");
    LOG_TEST(QString("redirecttUrlData.count()=%1").arg(redirecttUrlData.count()));
    if(redirecttUrlData.count() != 1)
    {
        return false;
    }
    QStringList redirectUrlCapture = redirecttUrlData.first();
    LOG_TEST(QString("redirectUrlCapture.count()=%1").arg(redirectUrlCapture.count()));
    if(redirectUrlCapture.count() <= 2)
    {
        return false;
    }
    QString redirectUrl = redirectUrlCapture.at(2);
    LOG_TEST(QString("redirectUrl=%1").arg(redirectUrl));

    m_scan = parseUrlParam(redirectUrl,"scan");
    m_ticket = parseUrlParam(redirectUrl,"ticket");
    return !(m_scan.isEmpty() || m_ticket.isEmpty());
}

bool CLoginManager::parseInitData(const QByteArray &byteArray)
{
    QJsonParseError errorString;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray,&errorString);
    LOG_TEST(QString("errorString = %1").arg(errorString.errorString()));
    QVariantMap objMap = doc.toVariant().toMap();
    LOG_TEST(QString("objMap.keys()=%1").arg(QStringList(objMap.keys()).join("-")));
    QString key = "User";
    if(objMap.contains(key))
    {
        QVariantMap userObjMap = objMap.value(key).toMap();
        LOG_TEST(QString("userObjMap.keys()=%1").arg(QStringList(userObjMap.keys()).join("-")));
        m_userData = Z_WX_USER_DATA::parseMap(userObjMap);
//        m_userAvatar.m_url = m_userData.HeadImgUrl;
//        m_userAvatar.m_userName = m_userData.NickName;
//        LOG_TEST(QString("m_userAvatar.m_userName = %1").arg(m_userAvatar.m_userName));
//        LOG_TEST(QString("m_userAvatar.m_url = %1").arg(m_userAvatar.m_url));
//        m_userAvatar.requestAvatar();
    }
    key = "SyncKey";
    if(objMap.contains(key))
    {
        m_syncKeyList = Z_WX_SyncKeyList::parseList(objMap.value(key).toMap().value("List").toList());
        LOG_TEST(QString("m_syncKeyList.itemList.count = %1").arg(m_syncKeyList.itemList.count()));
    }
    key = "ContactList";
    if(objMap.contains(key))
    {
        QVariantList objList = objMap.value(key).toList();
        for(auto d:objList)
        {
            Z_WX_USER_DATA obj = Z_WX_USER_DATA::parseMap(d.toMap());
            if(obj.UserName.startsWith("@@") && !m_groupNameList.contains(obj.UserName))
            {
                m_groupNameList.append(obj.UserName);
            }
        }
        LOG_TEST(QString("m_groupNameList.count = %1").arg(m_groupNameList.count()));
    }
    key = "ChatSet";
    if(objMap.contains(key))
    {
        QString keyString = objMap.value(key).toString();
        QStringList keyList = keyString.split(",");
        for(auto d:keyList)
        {
            if(d.startsWith("@@") && !m_groupNameList.contains(d))
            {
                m_groupNameList.append(d);
            }
        }

    }
    LOG_TEST(QString("m_groupNameList.count = %1").arg(m_groupNameList.count()));
    return true;
}

QString CLoginManager::parseUrlParam(const QString &url, const QString &param)
{
    QList<QStringList> paramData = Zpublic::regexCapture(url,QString("(%1)=([\\w\\-\\.,@?^=%;:/~\\+#]+)?").arg(param));
    QString result;
    if(paramData.isEmpty())
    {
        return result;
    }
    QStringList paramCapture = paramData.first();
    if(paramCapture.count() <= 2)
    {
        return result;
    }
    result = paramCapture.at(2);
    return result;
}

void CLoginManager::doRequestFinished(const CPB::RequestReplyData &response)
{
    if(response.statusCode > 200)
    {
        LOG_TEST(QString("request is error").arg(QString(response.replyData)));

        if(response.statusCode == 408 && response.type == TYPE_REQUEST_WAIT_LOGIN)
        {
            requestUuid();
        }
        return;
    }
    switch (response.type)
    {
    case HttpRequestType::TYPE_REQUEST_LOGIN_UUID:
    {
        m_uuid = parseUuid(response.replyData);
        m_status = CPB::LOGIN_STATUS_LOGINING;
        emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_LOGIN_STATUS,m_status);
        requestQrCode();
        break;
    }
    case HttpRequestType::TYPE_REQUEST_QR_CODE:
    {
        m_image = QImage::fromData(response.replyData);
        emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_QR_CODE,m_image);
        m_tip = 1;
        m_status = CPB::LOGIN_STATUS_WATING_SCAN;
        emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_LOGIN_STATUS,m_status);
        onRequestWaitLogin();
        break;
    }
    case HttpRequestType::TYPE_REQUEST_WAIT_LOGIN:
    {
        QString res = response.replyData;
        if(res.isEmpty())
        {
            m_lastError = "replyData is empty";
            m_status = CPB::LOGIN_STATUS_ERROR;
            emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_LOGIN_STATUS,m_status);
        }
        else if(res.contains("window.code=201"))
        {
            m_tip = 0;
            m_status = CPB::LOGIN_STATUS_SCAN_SUCCESSFUL;
            emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_LOGIN_STATUS,m_status);
            if(!m_timer.isActive())
            {
                m_timer.start();
            }
        }
        else if(res.contains("window.code=200"))
        {
            bool ok = parseRedirectUri(response.replyData);
            if(ok)
            {
                m_status = CPB::LOGIN_STATUS_REQUEST_COOKIE;
                requestCookie();
            }
            else
            {
                m_status = CPB::LOGIN_STATUS_ERROR;
                m_lastError = "解析错误";
            }
            emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_LOGIN_STATUS,m_status);
        }

        break;
    }
    case HttpRequestType::TYPE_REQUEST_COOKIE:
    {
        bool ok = parseCookieData(response.replyData,m_cookieParam);
//        emit sigLoginFinished(ok);
        if(ok)
        {
            QString str15("e");
            for(int i = 0; i < 15;++i)
            {
                str15.append(QString::number(qrand()%10));
            }
            m_baseRequestParam.deviceID = str15;
            m_baseRequestParam.skey = m_cookieParam.skey;
            m_baseRequestParam.sid = m_cookieParam.wxsid;
            m_baseRequestParam.uin = m_cookieParam.wxuin;
            m_baseRequestParam.pass_ticket = m_cookieParam.pass_ticket;

            m_status = CPB::LOGIN_STATUS_INITIALIZING;
            emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_LOGIN_STATUS,m_status);
            requestInit();
        }
        else
        {
            m_lastError = "parseCookieData is error";
            m_status = CPB::LOGIN_STATUS_ERROR;
            emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_LOGIN_STATUS,m_status);
        }
        break;
    }
    case HttpRequestType::TYPE_REQUEST_WX_INIT:
    {
        bool ok = parseInitData(response.replyData);
        if(ok)
        {
            m_status = CPB::LOGIN_STATUS_LOGIN_SUCCESSFUL;
            emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_LOGIN_STATUS,m_status);
            requestStatusNotify();
        }
        break;
    }
    case TYPE_REQUEST_STATUS_NOTIFY:
    {
        CContactManager::getInstance()->requestContact(m_baseRequestParam);
        CContactManager::getInstance()->requestContactGroup(m_baseRequestParam,m_groupNameList);
        break;
    }
    default:
        break;
    }
    LOG_TEST(QString(response.replyData));
    return;
}

