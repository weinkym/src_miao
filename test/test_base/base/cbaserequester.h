#ifndef CBASEREQUESTER_H
#define CBASEREQUESTER_H

#include "cbaserequestaction.h"
#include <QObject>

class CBaseRequester : public QObject
{
    Q_OBJECT
public:
    CBaseRequester();
    ~CBaseRequester();

protected:
    QUuid request(CBaseRequestAction *action);
    virtual void doRequestFinished(bool ok, const CBaseRequestAction::Data &data) = 0;

private slots:
    void onRequestFinished(const CBaseRequestAction::Data &data);
};

#endif // CBASEREQUESTER_H
