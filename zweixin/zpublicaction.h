#ifndef ZPUBLICACTION_H
#define ZPUBLICACTION_H

#include "zrequestaction.h"

struct ZBaseRequestParam
{
    QString skey;
    QString sid;
    QString uin;
    QString deviceID;
    QString pass_ticket;
};

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

protected:
    ZPublicAction(HttpRequestType type);
    Operation getOperation();
    QNetworkRequest getRequest() const;
    virtual QByteArray getByteArray() const;

private:
    QString m_uuid;
    QString m_ticket;
    QString m_scan;
    QString m_url;
    ZBaseRequestParam m_baseRequestParam;
    int m_tip;
};

#endif // ZPUBLICACTION_H
