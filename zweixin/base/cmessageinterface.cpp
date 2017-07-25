#include "cmessageinterface.h"
#include "csqliteaccessinterface.h"
#include "zgolbal.h"

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
    m_initialized = true;
    for(auto v:model)
    {
        CPB::AutoSendEventData obj = CPB::AutoSendEventData::parseMap(v.toMap());
        m_messageMap.insert(obj.uuid,obj);
    }
    LOG_INFO(QString("message count = %1").arg(m_messageMap.count()));
}

void CMessageInterface::addMessage(const CPB::AutoSendEventData &msg)
{
    if(m_messageMap.contains(msg.uuid))
    {
        return;
    }
    m_messageMap.insert(msg.uuid,msg);
}

CMessageInterface::CMessageInterface(QObject *parent)
    :QObject(parent)
    ,m_initialized(false)
    ,m_timer(NULL)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(5 * 60 * 1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTimerout()));
}

void CMessageInterface::onTimerout()
{
    //TODO
}
