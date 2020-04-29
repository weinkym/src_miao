#ifndef CBASEREQUESTACTION_H
#define CBASEREQUESTACTION_H

#include "clogsetting.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTimer>
#include <QUuid>
#include <QVariantMap>

class CBaseRequestAction : public QObject
{
    Q_OBJECT
public:
    enum RequestType
    {
        REQUEST_TYPE_UNDEFINED = 0, /*!< 未定义操作 */
        REQUEST_TYPE_POST,
        REQUEST_TYPE_POST_MULTI_PART, /*!< 发送QHttpMultiPart */
        REQUEST_TYPE_HEAD,
        REQUEST_TYPE_GET,
        REQUEST_TYPE_DELETE,
        REQUEST_TYPE_PATCH,
        REQUEST_TYPE_PUT
    };
    struct Data
    {
        Data();
        int networkErrorCode;
        int statusCode;

        QString actionName;
        QUuid uuid;
        QDateTime startTime;
        QDateTime endTime;
        QByteArray replyArray;
        QVariant value;
        QVariant errorValue;
        QVariant customValue;
        QHash<QByteArray, QByteArray> rawHeaderMap;
    };

    explicit CBaseRequestAction(QObject *parent = 0);
    virtual ~CBaseRequestAction();
    //    CBaseRequestAction::RequestType getType() const;
    QUuid getUuid() { return m_replayStatusData.uuid; }

protected:
    virtual QString getActionName() const = 0;
    virtual RequestType getRequestType() const = 0;
    virtual QNetworkRequest getRequest() const = 0;
    virtual QByteArray getByteArray() const;
    virtual QHttpMultiPart *getMultiPart();
    virtual void doPreviousReplyFinished();

private:
    void initTimeoutTimer();
    void cancel();
    void startRequestAction();

protected slots:
    void onReplyFinished();
    void onReplyError(QNetworkReply::NetworkError error);
    void onIgnoreSSlErrors(const QList<QSslError> &sslErrors);
    void onHttpTimeout();
    void onCancel(const QUuid &uuid);

signals:
    void sigCancel();
    void sigRequestFinished(const CBaseRequestAction::Data &data);

protected:
    static const int m_defaultHttpTimeout = 30000;
    static const int m_defaultMaxRetryTime = 3;

    friend class CRequestAccessManager;
    friend class CBaseRequester;
    QNetworkReply *m_networkReply;
    QTimer *m_timerTimeout;
    int m_httpTimeout;
    int m_maxRetryTime;
    int m_curRetryTime;
    Data m_replayStatusData;
};

#endif // CBASEREQUESTACTION_H
