#ifndef CMESSAGEINTERFACE_H
#define CMESSAGEINTERFACE_H
#include <QObject>
#include <QTimer>
#include "zpublic.h"

class CMessageInterface : QObject
{
    Q_OBJECT

public:
    static CMessageInterface *getInstance();
    void addMessage(const CPB::AutoSendEventData &msg);

protected:
    CMessageInterface(QObject *parent = NULL);

protected slots:
    void onTimerout();

private:
    static CMessageInterface *m_instance;
    QMap<QString,CPB::AutoSendEventData> m_messageMap;
    QTimer *m_timer;
};

#endif // CMESSAGEINTERFACE_H
