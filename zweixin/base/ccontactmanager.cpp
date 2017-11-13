#include "ccontactmanager.h"
#include "zpublicaction.h"
#include <QJsonDocument>
#include "cloginmanager.h"
#include "cmessageinterface.h"

CContactManager * CContactManager::m_instance = NULL;

CContactManager *CContactManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CContactManager;
    }
    return m_instance;
}

CContactManager::~CContactManager()
{

}

void CContactManager::sendMessage(const QString &toUserName, const QString &message)
{
    if(!m_contackMap.contains(toUserName) && !m_groupMap.contains(toUserName))
    {
        ZW_LOG_WARNING(QString("toUserName=%1 not found").arg(toUserName));
        return;
    }
    ZPublicAction *action = ZPublicAction::createSendMessage(CLoginManager::getInstance()->m_baseRequestParam,CLoginManager::getInstance()->m_userData.UserName,
                                                             toUserName,message);
    connectAction(action);
}

void CContactManager::requestContact(const ZBaseRequestParam &baseRequestParam)
{
    connectAction(ZPublicAction::createGetContact(baseRequestParam));
}

void CContactManager::requestContactGroup(const ZBaseRequestParam &baseRequestParam, const QStringList &groupNameList)
{
    connectAction(ZPublicAction::createGetGroup(baseRequestParam,groupNameList));
}

void CContactManager::requestWXSync(const ZBaseRequestParam &baseRequestParam, const Z_WX_SyncKeyList &syncKeyList)
{
    connectAction(ZPublicAction::createWXSync(baseRequestParam,syncKeyList));
}

QString CContactManager::getUserName(const QString &nickName)
{
    {
    QMapIterator<QString,QSharedPointer<Z_WX_USER_DATA> > iter(m_contackMap);
    while(iter.hasNext())
    {
        iter.next();
        if(iter.value().data()->NickName == nickName)
        {
            return iter.key();
        }
    }
    }
    {
    QMapIterator<QString,QSharedPointer<Z_WX_USER_DATA> > iter(m_groupMap);
    while(iter.hasNext())
    {
        iter.next();
        if(iter.value().data()->NickName == nickName)
        {
            return iter.key();
        }
    }
    }
    return "";
}

CContactManager::CContactManager(QObject *parent)
    :CBaseObject(parent)
{
    for(int i = 0; i <100; ++i)
    {
        QSharedPointer<Z_WX_USER_DATA> pObj(new Z_WX_USER_DATA);
        pObj.data()->UserName = QUuid::createUuid().toString();
        pObj.data()->NickName = QString("TEMP%1").arg(i);
        m_contackMap.insert(pObj.data()->UserName,pObj);
    }
}

void CContactManager::doRequestFinished(const CPB::RequestReplyData &response)
{
    if(response.statusCode > 200)
    {
        ZW_LOG_DEBUG(QString("request is error").arg(QString(response.replyData)));
        if(response.type == TYPE_REQUEST_WX_SYNC)
        {
            QTimer::singleShot(1000, [this]()
            {
                this->requestWXSync(CLoginManager::getInstance()->m_baseRequestParam,CLoginManager::getInstance()->m_syncKeyList);
            });
        }
        return;
    }

    switch (response.type)
    {
    case TYPE_REQUEST_CONTACT:
    {
        QJsonParseError errorString;
        QJsonDocument doc = QJsonDocument::fromJson(response.replyData,&errorString);
        ZW_LOG_DEBUG(QString("errorString = %1").arg(errorString.errorString()));
        QVariantMap objMap = doc.toVariant().toMap();
        ZW_LOG_DEBUG(QString("objMap.keys()=%1").arg(QStringList(objMap.keys()).join("-")));
        QVariantList objList = objMap.value("MemberList").toList();
        ZW_LOG_DEBUG(QString("objList.count = %1").arg(objList.count()));
        m_contackMap.clear();
        for(auto obj:objList)
        {
            Z_WX_USER_DATA d = Z_WX_USER_DATA::parseMap(obj.toMap());
            //"ContactFlag": 1-公众号， 2-群组， 3-好友
            if(!m_contackMap.contains(d.UserName) && d.VerifyFlag == 0)
            {
                Z_WX_USER_DATA *pObj = new Z_WX_USER_DATA;
                *pObj = d;
                m_contackMap.insert(d.UserName,QSharedPointer<Z_WX_USER_DATA>(pObj));
                ZW_LOG_DEBUG(QString("UserName = %1,NickName = %2,VerifyFlag = %3").arg(pObj->UserName).arg(pObj->NickName).arg(d.VerifyFlag));
            }
        }
        emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_CONTACT_LIST,"");
        ZW_LOG_DEBUG(QString("m_contackMap.count = %1").arg(m_contackMap.count()));
       CMessageInterface::getInstance()->init();
       break;
    }
    case TYPE_REQUEST_GROUP:
    {
        QJsonParseError errorString;
        QJsonDocument doc = QJsonDocument::fromJson(response.replyData,&errorString);
        ZW_LOG_DEBUG(QString("errorString = %1").arg(errorString.errorString()));
        QVariantMap objMap = doc.toVariant().toMap();
        ZW_LOG_DEBUG(QString("objMap.keys()=%1").arg(QStringList(objMap.keys()).join("-")));
        QVariantList objList = objMap.value("ContactList").toList();
        ZW_LOG_DEBUG(QString("objList.count = %1").arg(objList.count()));
        m_groupMap.clear();
        for(auto obj:objList)
        {
            Z_WX_USER_DATA d = Z_WX_USER_DATA::parseMap(obj.toMap());
            if(!m_groupMap.contains(d.UserName)/* && d.ContactFlag == 3*/)
            {
                Z_WX_USER_DATA *pObj = new Z_WX_USER_DATA;
                *pObj = d;
                m_groupMap.insert(d.UserName,QSharedPointer<Z_WX_USER_DATA>(pObj));
                ZW_LOG_DEBUG(QString("UserName = %1,NickName = %2,VerifyFlag = %3").arg(pObj->UserName).arg(pObj->NickName).arg(d.VerifyFlag));
            }
        }
        emit sigDateUpdate(CPB::DATA_UPDATE_TYPE_GROUP_LIST,"");
        ZW_LOG_DEBUG(QString("m_contackMap.count = %1").arg(m_groupMap.count()));
        //TODO
        ZW_LOG_DEBUG(QString("CLoginManager::getInstance()->requestSyncCheck()"));
        CLoginManager::getInstance()->requestSyncCheck();
        break;
    }
    case TYPE_REQUEST_WX_SYNC:
    {
        QJsonParseError errorString;
        QJsonDocument doc = QJsonDocument::fromJson(response.replyData,&errorString);
        ZW_LOG_DEBUG(QString("errorString = %1").arg(errorString.errorString()));
        QVariantMap objMap = doc.toVariant().toMap();
        ZW_LOG_DEBUG(QString("objMap.keys()=%1").arg(QStringList(objMap.keys()).join("-")));
        {
            QVariantList AddMsgList = objMap.value("AddMsgList").toList();
            for(auto d:AddMsgList)
            {
                Z_WX_MSG_DATA msg = Z_WX_MSG_DATA::parseMap(d.toMap());
                ZW_LOG_DEBUG(QString("msg:id=%1,MsgType=%2,content=%3,appinfo.AppID=%4,RecommendInfo.UserName=%5")
                         .arg(msg.NewMsgId).arg(msg.MsgType).arg(msg.Content).arg(msg.AppInfo.AppID).arg(msg.RecommendInfo.UserName));
            }
        }
        CLoginManager::getInstance()->m_syncKeyList = Z_WX_SyncKeyList::parseList(objMap.value("SyncCheckKey").toMap().value("List").toList());
        if(CLoginManager::getInstance()->m_syncKeyList.itemList.isEmpty())
        {
            ZW_LOG_CRITICAL(QString("SyncCheckKey parse is error"));
            exit(-12);;
        }
        CLoginManager::getInstance()->requestSyncCheck();
        break;
    }
    default:
        break;
    }
    return;
}

