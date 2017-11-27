#include "cmessageinterface.h"
#include "csqliteaccessinterface.h"
#include "zgolbal.h"
#include "ccontactmanager.h"
#include "zwcore.h"
#include "zpublicaction.h"
#include "cloginmanager.h"

CMessageInterface *CMessageInterface::m_instance = NULL;
CMessageInterface *CMessageInterface::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CMessageInterface;
    }
    return m_instance;
}

void CMessageInterface::init()
{
    if(m_initialized)
    {
        return;
    }
    QString errorString;
    QVariantList model;
    bool ok = CSqliteAccessInterface::getInstance()->queryAllMessage(model,&errorString);
    if(!ok)
    {
        return;
    }
//    m_initialized = true;
    m_messageMap.clear();
    for(auto v:model)
    {
        CPB::AutoSendEventData obj = CPB::AutoSendEventData::parseMap(v.toMap());
        m_messageMap.insert(obj.uuid,obj);
    }
    ZW_LOG_INFO(QString("message count = %1").arg(m_messageMap.count()));
    m_timer->start();
}

void CMessageInterface::addMessage(const CPB::AutoSendEventData &msg)
{
    ZW_LOG_FUNCTION;
    if(m_messageMap.contains(msg.uuid))
    {
        ZW_LOG_WARNING(QString("addMessage is exist uuid=%1").arg(msg.uuid));
        return;
    }
    ZW_LOG_INFO(QString("addMessage uuid=%1,content=%2").arg(msg.uuid).arg(msg.content));
    m_messageMap.insert(msg.uuid,msg);
    CSqliteAccessInterface::getInstance()->insertMessage(msg);
}

void CMessageInterface::deleteMessage(const QString &uuid)
{
    ZW_LOG_FUNCTION;
    if(m_messageMap.contains(uuid))
    {
        ZW_LOG_INFO(QString("remove message uuid=%1").arg(uuid));
        m_messageMap.remove(uuid);
        CSqliteAccessInterface::getInstance()->deleteMessage(uuid);
    }
    else
    {
        ZW_LOG_WARNING(QString("deleteMessage is not exist uuid=%1").arg(uuid));
    }
}

void CMessageInterface::setSendNickNames(const QStringList &sendNickNames)
{
    m_sendNickNames = sendNickNames;
    ZW_LOG_INFO(QString("update sendNickNames=%1").arg(sendNickNames.join("<---->")));
}

void CMessageInterface::start()
{
    if(m_timer)
    {
        m_timer->start();
    }
}

void CMessageInterface::stop()
{
    if(m_timer)
    {
        m_timer->stop();
    }
}

bool CMessageInterface::containNickName(const QString &nickName)
{
    return m_sendNickNames.contains(nickName);
}

CMessageInterface::CMessageInterface(QObject *parent)
    :CBaseObject(parent)
    ,m_initialized(false)
    ,m_timer(NULL)
{
    m_timer = new QTimer(this);
//    m_timer->setInterval(m_intervalSeconds * 1000);
    m_sendNickNames.append("TT123456");
    m_timer->setInterval(1 * 60 * 1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTimerout()));
}

void CMessageInterface::doRequestFinished(const CPB::RequestReplyData &response)
{
    if(response.statusCode > 200)
    {
        ZW_LOG_WARNING(QString("request is error").arg(QString(response.replyData)));
        if(response.type == TYPE_REQUEST_WX_SEND_MSG)
        {
            ZW_LOG_WARNING(QString("send message is error %1").arg(QString(response.replyData)));
        }
        return;
    }

    switch (response.type)
    {
    case TYPE_REQUEST_WX_SEND_MSG:
    {
        ZW_LOG_INFO(QString("send message is ok %1").arg(QString(response.replyData)));
        break;
    }

    default:
        break;
    }
    return;
}

void CMessageInterface::onTimerout()
{
    ZW_LOG_FUNCTION;
//    init();
    //===========
    QDateTime currentDateTime = QDateTime::currentDateTime();
    bool isHourly  = currentDateTime.time().minute() <= (m_intervalSeconds / 50);
    bool sendHourRang = (currentDateTime.time().hour() >= 8);
    ZW_LOG_INFO(QString("time:%1,isHourly=%2,sendHourRang=%3")
                .arg(currentDateTime.toString("yyyyMMdd hh:mm:ss"))
                .arg(ZWCore::getBoolString(isHourly))
                .arg(ZWCore::getBoolString(sendHourRang)));
    ZW_LOG_INFO(QString("m_messageMap.count=%1").arg(m_messageMap.count()));

    QMapIterator<QString,CPB::AutoSendEventData> iter(m_messageMap);
    while(iter.hasNext())
    {
        iter.next();
        QDateTime dateTime = QDateTime::fromTime_t(iter.value().dateTime);
        ZW_LOG_INFO(QString("dateTime:%1,iter.value().type=%2")
                    .arg(dateTime.toString("yyyyMMdd hh:mm:ss"))
                    .arg(iter.value().type));
        bool needSend = false;
        switch (iter.value().type)
        {
        case Zpublic::AUTO_SEND_EVENT_TYPE_DATE:
        {
            needSend = (isHourly && sendHourRang && dateTime.date() <= currentDateTime.date());
            break;
        }
        case Zpublic::AUTO_SEND_EVENT_TYPE_DATE_TIME:
        {
            needSend = (dateTime <= currentDateTime);
            break;
        }
        default:
            break;
        }
        if(needSend)
        {
            QString content = QString("%1:%2").arg(iter.key()).arg(iter.value().content);
            ZW_LOG_INFO(QString("sendmessage content = %1").arg(content));
            sendMessage(content);
        }
    }
}

void CMessageInterface::sendStatusMessage()
{
    ZW_LOG_FUNCTION;
    QString content=QString("check_status sendNickNames=%1").arg(m_sendNickNames.join("--"));
    QString messageList;
    QMapIterator<QString,CPB::AutoSendEventData> iter(m_messageMap);
    int index = 1;
    while(iter.hasNext())
    {
        iter.next();
        QDateTime dateTime = QDateTime::fromTime_t(iter.value().dateTime);
        messageList.append("@=========="+QString::number(index++)+"\n【TEXT="+iter.value().content+
                           "】\n【"+dateTime.toString("yyyyMMdd hh:mm:ss")+
                           "】\n【type="+QString::number(iter.value().type)+"】@MSG)\n");

    }
    content.append("\n"+messageList);
    this->sendMessage(content);
}

void CMessageInterface::sendMessage(const QString &content)
{
    foreach (const QString &nickName, m_sendNickNames)
    {
        QString toUserName = CContactManager::getInstance()->getUserName(nickName);
        if(!toUserName.isEmpty())
        {
            ZW_LOG_INFO(QString("sendmessage to nickName %1").arg(nickName));
            ZW_LOG_INFO(QString("sendmessage to toUserName %1").arg(toUserName));
            this->sendMessage(toUserName,content);
        }
        else
        {
            ZW_LOG_WARNING(QString("sendmessage nickName=%1 toUserName is empty").arg(nickName));
        }
    }
}

void CMessageInterface::sendMessage(const QString &toUserName, const QString &content)
{
    if(CContactManager::getInstance()->isUserValid(toUserName))
    {
        ZW_LOG_WARNING(QString("toUserName=%1 not found").arg(toUserName));
        return;
    }
    ZPublicAction *action = ZPublicAction::createSendMessage(CLoginManager::getInstance()->m_baseRequestParam,CLoginManager::getInstance()->m_userData.UserName,
                                                             toUserName,content);
    connectAction(action);
}
