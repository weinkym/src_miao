#include "cbaserequester.h"

CBaseRequester::CBaseRequester()
{
}

CBaseRequester::~CBaseRequester()
{
}

QUuid CBaseRequester::request(CBaseRequestAction *action)
{
    QUuid uuid;
    if(action)
    {
        connect(action, SIGNAL(sigRequestFinished(CBaseRequestAction::Data)), this, SLOT(onRequestFinished(CBaseRequestAction::Data)));
        action->startRequestAction();
        uuid = action->getUuid();
    }
    return uuid;
}

void CBaseRequester::onRequestFinished(const CBaseRequestAction::Data &data)
{
    bool ok = (data.statusCode == 200
        || data.statusCode == 201
        || data.statusCode == 202
        || data.statusCode == 204);
    doRequestFinished(ok, data);
}
