#ifndef CMESSAGEINTERFACE_H
#define CMESSAGEINTERFACE_H
#include <QObject>
#include <QTimer>
#include "zpublic.h"
#include "cbaseobject.h"

class CMessageInterface : CBaseObject
{
    Q_OBJECT

public:
    static CMessageInterface *getInstance();
    void init();
    void addMessage(const CPB::AutoSendEventData &msg);
    void deleteMessage(const QString &uuid);
    void deleteAllMessage();
    void setSendNickNames(const QStringList &sendNickNames);
    void start();
    void stop();
    bool containNickName(const QString &nickName);
    void sendStatusMessage();
    void sendMessage(const QString &content);
    void sendMessage(const QString &toUserName,const QString &content);

protected:
    CMessageInterface(QObject *parent = NULL);
    void doRequestFinished(const CPB::RequestReplyData &response);

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
