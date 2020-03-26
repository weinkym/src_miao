#ifndef CBASEOBJECT_H
#define CBASEOBJECT_H
#include <QObject>
#include "zpublic.h"

class ZRequestAction;
class CBaseObject : public QObject
{
    Q_OBJECT

public:
    CBaseObject(QObject *parent = NULL);

protected:
    void connectAction(ZRequestAction *action);
    virtual void doRequestFinished(const CPB::RequestReplyData &response) = 0;

private slots:
    void onRequestFinished(const CPB::RequestReplyData &response);

signals:
    void sigRequestFinished(const CPB::RequestReplyData &replayStatusData);
    void sigDateUpdate(int type,const QVariant &value);

};

#endif // CBASEOBJECT_H
