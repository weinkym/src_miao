#include "cbaseobject.h"
#include "zrequestaction.h"


CBaseObject::CBaseObject(QObject *parent)
    :QObject(parent)
{

}

void CBaseObject::connectAction(ZRequestAction *action)
{
    if(action == NULL)
    {
        return;
    }
    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SLOT(onRequestFinished(CPB::RequestReplyData)));
    connect(action,SIGNAL(sigRequestFinished(CPB::RequestReplyData)),this,SIGNAL(sigRequestFinished(CPB::RequestReplyData)));
    action->trigger();
}

void CBaseObject::onRequestFinished(const CPB::RequestReplyData &response)
{
    //
    doRequestFinished(response);
}
