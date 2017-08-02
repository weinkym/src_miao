#include "cmessageinterface.h"
#include "csqliteaccessinterface.h"
#include "zgolbal.h"
#include "ccontactmanager.h"

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
    LOG_INFO(QString("message count = %1").arg(m_messageMap.count()));
    m_timer->start();
}

void CMessageInterface::addMessage(const CPB::AutoSendEventData &msg)
{
    if(m_messageMap.contains(msg.uuid))
    {
        return;
    }
    m_messageMap.insert(msg.uuid,msg);
    CSqliteAccessInterface::getInstance()->insertMessage(msg);
}

CMessageInterface::CMessageInterface(QObject *parent)
    :QObject(parent)
    ,m_initialized(false)
    ,m_timer(NULL)
{
    m_timer = new QTimer(this);
//    m_timer->setInterval(m_intervalSeconds * 1000);
    m_timer->setInterval(5 * 1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTimerout()));
}

void CMessageInterface::onTimerout()
{
    LOG_FUNCTION;
//    init();
    //===========
    QDateTime currentDateTime = QDateTime::currentDateTime();
    bool isHourly  = currentDateTime.time().minute() <= (m_intervalSeconds / 50);
    QMapIterator<QString,CPB::AutoSendEventData> iter(m_messageMap);
    while(iter.hasNext())
    {
        iter.next();
        QDateTime dateTime = QDateTime::fromTime_t(iter.value().dateTime);
//        LOG_INFO(QString("currentDateTime = %1,dateTime=%2")
//                 .arg(currentDateTime.toString("yyyyMMdd hh:mm:ss"))
//                 .arg(dateTime.toString("yyyyMMdd hh:mm:ss")));
        LOG_INFO(QString("TTTTTTTTTTT"));
        bool needSend = false;
        switch (iter.value().type)
        {
        case Zpublic::AUTO_SEND_EVENT_TYPE_MONTH:
        {
            needSend = (isHourly && dateTime.date().day() == currentDateTime.date().day());
            break;
        }
        case Zpublic::AUTO_SEND_EVENT_TYPE_DAY:
        {
            needSend = (isHourly && dateTime <= currentDateTime);
            break;
        }
        case Zpublic::AUTO_SEND_EVENT_TYPE_HOUR:
        {
            needSend = isHourly;
            break;
        }
        case Zpublic::AUTO_SEND_EVENT_TYPE_NORMAL:
        {
            needSend = (isHourly && dateTime.time() <= currentDateTime.time());
            break;
        }
        default:
            break;
        }
        if(needSend)
        {
            QString content = QString("%1:%2").arg(iter.key()).arg(iter.value().content);
            LOG_INFO(QString("content = %1").arg(content));
            QString toUserName = CContactManager::getInstance()->getUserName("TT123456");
            CContactManager::getInstance()->sendMessage(toUserName,content);
        }
    }
}
