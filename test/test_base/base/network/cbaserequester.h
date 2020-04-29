#ifndef CBASEREQUESTER_H
#define CBASEREQUESTER_H

#include "cbaserequestaction.h"
#include <QObject>

class CBaseRequester : public QObject
{
    Q_OBJECT
public:
    CBaseRequester(QObject *parent = nullptr);
    ~CBaseRequester();

protected:
    QUuid request(CBaseRequestAction *action);
    virtual void doRequestFinished(bool ok, const CBaseRequestAction::Data &data) = 0;
    void stopReqeust(const QUuid &uuid);

private slots:
    void onRequestFinished(const CBaseRequestAction::Data &data);

signals:
    void sigCancel(const QUuid &id);
};

#endif // CBASEREQUESTER_H
