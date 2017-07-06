#ifndef CLOGINMANAGER_H
#define CLOGINMANAGER_H

#include "cbaseobject.h"
#include <QTimer>
#include <QImage>

class CLoginManager : public CBaseObject
{
    Q_OBJECT
public:
    static CLoginManager *getInstance();
    ~CLoginManager();
    void start();
    void requestSyncCheck();

protected:
    CLoginManager(QObject *parent = NULL);
    void doRequestFinished(const CPB::RequestReplyData &response);

private:
    void requestUuid();
    void requestQrCode();
    void requestCookie();
    void requestInit();
    void requestStatusNotify();

    static QString parseUuid(const QByteArray &byteArray);
    static bool parseCookieData(const QByteArray &byteArray,Z_WX_COOKIE_PARAM &param);
    bool parseRedirectUri(const QByteArray &byteArray);
    bool parseInitData(const QByteArray &byteArray);
    QString parseUrlParam(const QString &url,const QString &param);


private slots:
    void onRequestWaitLogin();

public:
    static CLoginManager *m_instance;
    QString m_uuid;
//    QString m_redirectUrl;
    QString m_scan;
    QString m_ticket;

    QImage m_image;
    Z_WX_COOKIE_PARAM m_cookieParam;
    ZBaseRequestParam m_baseRequestParam;
    QTimer m_timer;
    int m_tip;
    QString m_lastError;
    CPB::LOGIN_STATUS m_status;
    Z_WX_USER_DATA m_userData;
    Z_WX_SyncKeyList m_syncKeyList;
    QStringList m_groupNameList;
    bool m_isStarting;
    bool m_isSyncChecking;
};

#endif // CLOGINMANAGER_H
