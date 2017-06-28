#ifndef ZREQUESTACTION_H
#define ZREQUESTACTION_H


#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariantMap>
#include <QUuid>
#include <QTimer>
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariantMap>
#include <QUuid>
#include <QTimer>

#include "zgolbal.h"
#include "zpublic.h"

/*! \brief 实现了CRequestAction类,用于网络模块通信动作基类。
 *类主要用于抽象通信动作的接口和共同实现
 */

class ZRequestAction : public QObject
{
    Q_OBJECT
public:
    /*! \brief 枚举, 定义Http协议操作
     */
    enum Operation
    {
        Undefine = 0, /*!< 未定义操作 */
        PostByteArray, /*!< 发送文本信息 */
        PostWithttachment, /*!< 发送QHttpMultiPart */
        Get, /*!< 请求数据 */
        Delete, /*!< 删除数据 */
        PatchByteArray, /*!< 更新文本数据 */
        PatchMultimedia,/*!< 更新多媒体数据 */
        PutByteArray
    };

    //! Http返回的状态码
    enum HttpStatesCode
    {
        OK = 200,             /*!< 成功 */
        PartialContent = 206,  /*!< 断点续传 */
        NotChanged = 304     /*!< 所请求资源无变化 */
    };

    /*! \brief 构造函数
     *  \param parent 父对象指针
     */
    explicit ZRequestAction(HttpRequestType type, QObject *parent = 0);

    /*! \brief 析构函数
     */
    virtual ~ZRequestAction();

    /*! \brief 触发请求动作
     */
    void startRequestAction();

    /*! \brief 触发网络通信动作
     */
    void trigger();

    /*! \brief 获取当前动作类型
     *  \return 返回动作类型
     */
    int getType() const;

    /*! \brief 获取当前动作使用的Http请求头
     *  \return 返回Http请求头
     */
    virtual QNetworkRequest createRequest() const = 0;
    QUuid getUuid() { return m_replayStatusData.uuid; }

protected:
    virtual Operation getOperation();

    virtual QByteArray getByteArray() const;


    virtual QHttpMultiPart *getMultiPart();



    int parseErrorCode(const QVariantMap &result);


    QString parseErrorMsg(const QVariantMap &result);


    void setType(HttpRequestType type);

    QString createBoundary();

    void requestFromHttp();

    QString description();

private:
    void setReply(QNetworkReply *reply);
    void initTimeoutTimer();

protected slots:
    void onReplyFinished();

    virtual void onReplyError(QNetworkReply::NetworkError error);

    void onIgnoreSSlErrors(QList<QSslError> sslErrors);

    void onHttpTimeout();

signals:
    //按目前的设计, action在子线程运作，删除action需要在主线程里执行
    void sigRelease(QObject* action);
    void sigRequestFinished(const CPB::RequestReplyData &replayStatusData);

protected:
    friend class ZRequestAccessManager;
    QNetworkReply *m_networkReply; /*!< 用来访问网络请求结果的指针 */

    int m_httpTimeout;  /*!< http超时毫秒 */
    int m_maxRetryTime;  /*!< 网络错误引发的网络命令重试最大次数 */
    int m_curRetryTime /*!< 当前重试次数 */;
    bool m_msgSendCancelled; /*!< 当前的发送消息是否已取消 */
    bool m_isPriorityFromDB; /*!< 是否优先从数据库获取 */
    QTimer *m_timerTimeout; /*!< 网络请求超时的timer */

    CPB::RequestReplyData m_replayStatusData;
    static const int m_defaultHttpTimeout = 30000;  /*!< 默认http超时毫秒 */
    static const int m_defaultMaxRetryTime = 3; /*!< 默认网络错误引发的网络命令重试最大次数 */
    static QString m_secretKey;
    static QString m_appKey;
};

#endif // ZREQUESTACTION_H
