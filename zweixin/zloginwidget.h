#ifndef ZLOGINWIDGET_H
#define ZLOGINWIDGET_H

#include <QWidget>
#include "zgolbal.h"
#include "zpublicaction.h"
#include "zpublic.h"

namespace Ui {
class zloginwidget;
}

class ZLoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZLoginWidget(QWidget *parent = 0);
    ~ZLoginWidget();
    void requestUuid();
    Z_WX_COOKIE_PARAM getCookieParam(){return m_cookieParam;};

private:
    void requestQrCode(const QString &uuid);
    void requestWaitLogin(const QString &uuid,int tip);
    void requestCookie(const QString &uuid,const QString &ticket,const QString &scan);
    QString parseUuid(const QByteArray &byteArray);
    bool parseRedirectUri(const QByteArray &byteArray);
    QString parseUrlParam(const QString &url,const QString &param);
    bool parseCookieData(const QByteArray &byteArray);

private slots:
    void onRequestFinished(const CPB::RequestReplyData &response);

    void onRequestWaitLogin();

signals:
    void sigLoginFinished(bool);

private:
    Ui::zloginwidget *ui;
    QString m_uuid;
    QString m_redirectUrl;
    QString m_scan;
    QString m_ticket;
    int m_tip;
    QTimer m_timer;
    Z_WX_COOKIE_PARAM m_cookieParam;
};

#endif // ZLOGINWIDGET_H
