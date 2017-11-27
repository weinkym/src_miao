#ifndef CCONTACTMANAGER_H
#define CCONTACTMANAGER_H

#include "cbaseobject.h"

class CContactManager : public CBaseObject
{
    Q_OBJECT
public:
    static CContactManager *getInstance();
    ~CContactManager();
//    void sendMessage(const QString &toUserName,const QString &message);
    void requestContact(const ZBaseRequestParam &baseRequestParam);
    void requestContactGroup(const ZBaseRequestParam &baseRequestParam,const QStringList &groupNameList);
    void requestWXSync(const ZBaseRequestParam &baseRequestParam,const Z_WX_SyncKeyList &syncKeyList);
    QString getUserName(const QString &nickName);
    QString getNickName(const QString &userName);
    bool isUserValid(const QString &userName);

protected:
    CContactManager(QObject *parent = NULL);
    void doRequestFinished(const CPB::RequestReplyData &response);
    void doWXMessage(const Z_WX_MSG_DATA &msg);

public:
    static CContactManager *m_instance;
    QMap<QString,QSharedPointer<Z_WX_USER_DATA> >m_contackMap;
    QMap<QString,QSharedPointer<Z_WX_USER_DATA> >m_groupMap;
};

#endif // CCONTACTMANAGER_H
