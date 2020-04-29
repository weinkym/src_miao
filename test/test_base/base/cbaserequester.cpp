#include "cbaserequester.h"

CBaseRequester::CBaseRequester(QObject *parent)
    : QObject(parent)
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
        connect(this, SIGNAL(sigCancel(QUuid)), action, SLOT(onCancel(QUuid)));
        connect(action, SIGNAL(sigRequestFinished(CBaseRequestAction::Data)), this, SLOT(onRequestFinished(CBaseRequestAction::Data)));
        action->startRequestAction();
        uuid = action->getUuid();
    }
    return uuid;
}

void CBaseRequester::cancel(const QUuid &uuid)
{
    C_LOG_OUT_V(uuid);
    emit sigCancel(uuid);
}

void CBaseRequester::onRequestFinished(const CBaseRequestAction::Data &data)
{
    bool ok = (data.statusCode >= 200 && data.statusCode < 300);

    doRequestFinished(ok, data);
}
