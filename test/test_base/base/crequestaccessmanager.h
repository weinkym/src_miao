#ifndef CREQUESTACCESSMANAGER_H
#define CREQUESTACCESSMANAGER_H

#include <QBuffer>
#include <QNetworkAccessManager>
#include <QObject>

class CBaseRequestAction;

class CRequestAccessManager : public QObject
{
    Q_OBJECT

public:
    static CRequestAccessManager *getInstance();
    bool request(CBaseRequestAction *action);

protected:
    explicit CRequestAccessManager(QObject *parent = 0);

private:
    QNetworkReply *postByteArray(CBaseRequestAction *action);
    QNetworkReply *putByteArray(CBaseRequestAction *action);
    QNetworkReply *postWithttachment(CBaseRequestAction *action);
    QNetworkReply *head(CBaseRequestAction *action);
    QNetworkReply *get(CBaseRequestAction *action);
    QNetworkReply *patchByteArray(CBaseRequestAction *action);
    QNetworkReply *deleteResource(CBaseRequestAction *action);
    void recoverNetworkAccessible();

private slots:
    void onIgnoreSSlErrors(QNetworkReply *reply, QList<QSslError> sslErrors);

private:
    QNetworkAccessManager *m_access;
    static CRequestAccessManager *m_instance;
};

#endif // CREQUESTACCESSMANAGER_H
