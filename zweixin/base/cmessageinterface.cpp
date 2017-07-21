#include "cmessageinterface.h"

CMessageInterface *CMessageInterface::m_instance = NULL;
CMessageInterface *CMessageInterface::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CMessageInterface;
    }
    return m_instance;
}

void CMessageInterface::addMessage(const CPB::AutoSendEventData &msg)
{
    if(m_messageMap.contains(msg.uuid))
    {
        return;
    }
    m_messageMap.insert(msg.uuid,msg);
    //
}

CMessageInterface::CMessageInterface(QObject *parent)
    :QObject(parent)
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
