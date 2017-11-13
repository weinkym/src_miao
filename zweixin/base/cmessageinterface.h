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
    void init();
    void addMessage(const CPB::AutoSendEventData &msg);
    void deleteMessage(const QString &uuid);
    void setSendNickNames(const QStringList &sendNickNames);
    void start();
    void stop();
    bool containNickName(const QString &nickName);
    void sendStatusMessage();

protected:
    CMessageInterface(QObject *parent = NULL);

protected slots:
    void onTimerout();

private:
    static CMessageInterface *m_instance;
    static const int m_intervalSeconds = 300;
    bool m_initialized;
    QTimer *m_timer;
    QStringList m_sendNickNames;
    QMap<QString,CPB::AutoSendEventData>m_messageMap;
};

#endif // CMESSAGEINTERFACE_H
