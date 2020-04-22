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
    enum Operation
    {
        Undefine = 0, /*!< 未定义操作 */
        PostByteArray, /*!< 发送文本信息 */
        PostWithttachment, /*!< 发送QHttpMultiPart */
        Get, /*!< 请求数据 */
        Delete, /*!< 删除数据 */
        PatchByteArray, /*!< 更新文本数据 */
        PatchMultimedia, /*!< 更新多媒体数据 */
        PutByteArray
    };
    enum Type
    {
        TYPE_NODEFINED = 0,
        TYPE_REQUEST_SUMMARIES, //srs summaries message
        TYPE_REQUEST_ROOMS,
        TYPE_REQUEST_POST_SUMMARIES,
    };
    struct Data
    {
        Data();
        Type type;
        int networkErrorCode;
        int statusCode;

        QUuid uuid;
        QDateTime startTime;
        QDateTime endTime;
        QByteArray replyArray;
        QVariant value;
        QVariant errorValue;
        QVariant customValue;
    };

    explicit CBaseRequestAction(Type type, QObject *parent = 0);
    virtual ~CBaseRequestAction();
    CBaseRequestAction::Type getType() const;
    QUuid getUuid() { return m_replayStatusData.uuid; }

protected:
    virtual Operation getOperation() const = 0;
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
