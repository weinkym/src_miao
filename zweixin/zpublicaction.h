#ifndef ZPUBLICACTION_H
#define ZPUBLICACTION_H

#include "zrequestaction.h"

class ZPublicAction : public ZRequestAction
{
    Q_OBJECT
public:
    static ZPublicAction *createLoginUuidAction();
    static ZPublicAction *createQrCodeAction(const QString &uuid);
    static ZPublicAction *createWaitLoginAction(const QString &uuid,int tip);
    static ZPublicAction *createCookieAction(const QString &uuid, const QString &ticket,const QString &scan);
    static ZPublicAction *createWxInitAction(const ZBaseRequestParam &baseRequestParam);
    static ZPublicAction *createAvatarAction(const QString &url);
    static ZPublicAction *createStatusNotify(const ZBaseRequestParam &baseRequestParam,const QString &fromUserName,const QString &toUserName);
    static ZPublicAction *createGetContact(const ZBaseRequestParam &baseRequestParam);
    static ZPublicAction *createGetGroup(const ZBaseRequestParam &baseRequestParam,const QStringList &groupNameList);
    static ZPublicAction *createSendMessage(const ZBaseRequestParam &baseRequestParam,const QString &fromUserName,const QString &toUserName,const QString &message);

protected:
    ZPublicAction(HttpRequestType type);
    Operation getOperation();
    QNetworkRequest createRequest() const;
    virtual QByteArray getByteArray() const;

private:
    QString m_uuid;
    QString m_ticket;
    QString m_scan;
    QString m_url;
    QString m_fromUserName;
    QString m_toUserName;
    QString m_message;
    QStringList m_groupNameList;
    ZBaseRequestParam m_baseRequestParam;
    int m_tip;
};

#endif // ZPUBLICACTION_H
