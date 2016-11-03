#ifndef ZREQUESTACTION_H
#define ZREQUESTACTION_H


#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariantMap>
#include <QUuid>
#include <QTimer>
#include "zgolbal.h"

/*! \brief 实现了ZRequestAction类,用于网络模块通信动作基类。
 *类主要用于抽象通信动作的接口和共同实现
 */
enum HttpRequestType
{
    TYPE_REQUEST_LOGIN_UUID = 0,
    TYPE_REQUEST_QR_CODE,
    TYPE_REQUEST_WAIT_LOGIN,
    TYPE_REQUEST_COOKIE,
    TYPE_REQUEST_WX_INIT,
    TYPE_REQUEST_AVATAR,
    TYPE_REQUEST_UNDEFINED
};

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
    explicit ZRequestAction(HttpRequestType type, QObject *parent = 0);

    /*! \brief 析构函数
     */
    virtual ~ZRequestAction();

    /*! \brief 触发网络通信动作
     */
    void trigger();

    /*! \brief 获取当前动作类型
     *  \return 返回动作类型
     */
    HttpRequestType getType() const;


    /*! \brief 获取当前动作使用的Http请求头
     *  \return 返回Http请求头
     */
    virtual QNetworkRequest getRequest() const;


    struct ZRequestResponse
    {
        int code;
        int type;
        bool fromSever;
        QByteArray data;
    };

protected:
    /*! \brief 获取当动作使用的Http协议类型
     *  \return 返回Http协议类型
     */
    virtual Operation getOperation();

    /*! \brief 获取当前动作Http post的字符串数据
     *  \return 返回动作Http post的字符串数据
     */
    virtual QByteArray getByteArray() const;

    /*! \brief 获取当前动作Http post的多媒体数据
     *  \return 返回动作Http post的多媒体数据
     */
    virtual QHttpMultiPart *getMultiPart();

    /*! \brief 设置Http协议返回的二进制信息并解析
     *  \param dataArray http返回二进制信息
     */
    virtual void setReplyData(const QByteArray &dataArray, const QNetworkReply *reply);

    /*! \brief 创建Http请求头
     *  \param isOrgApi 是否是组织相关的api请求
     *  \param lastHeader 请求头地址
     */
    QNetworkRequest createRequest(const QString &lastHeader, bool isOrgApi = false,
                                  const QString &typeHeader = "application/json") const;

    /*! \brief 创建Http附件上传请求头
     *  \param lastHeader 请求头地址
     *  \param boundary 附件分割字符串
     */
    QNetworkRequest createAttachmentRequest(const QString &lastHeader, const QString &boundary);

    /*! \brief 解析错误服务器错误代码
     *  \param result 服务器响应键值对数据
     *  \return 服务器错误代码
     */
    int parseErrorCode(const QVariantMap &result);

    /*! \brief 解析错误服务器错误信息
     *  \param result 服务器响应键值对数据
     *  \return 服务器错误信息
     */
    QString parseErrorMsg(const QVariantMap &result);

    /*! \brief 获取附件类型
     *  \param suffix 附件后缀名
     *  \return 附件类型字符串
     */
    QString getAttachContentType(const QString &suffix);

    void setType(HttpRequestType type);

    QString createBoundary();
    virtual void requestFromDB();
    void requestFromHttp();

    void sendFinishedSignal(const quint64 errorCode,bool fromSever,const QByteArray &value,int type);
    QString description();

private:
    void setReply(QNetworkReply *reply);
    void initTimeoutTimer();

private slots:

    /*! \brief 公共槽函数,由网络请求服务器响应后触发
     */
    virtual void onReplyFinished();

    /*! \brief 槽函数
     *  reply 返回错误
     *  \param error 返回的错误
     */
    virtual void onReplyError(QNetworkReply::NetworkError error);

    /*! \brief 私有槽函数，忽略ssl错误
     *  \param reply network reply
     *  \param sslErrors 返回的ssl 错误
     */
    void onIgnoreSSlErrors(QList<QSslError> sslErrors);

    void onHttpTimeout();

signals:
    void sigFinished(const ZRequestAction::ZRequestResponse &response);

protected:
    friend class ZRequestAccessManager;
    QNetworkReply *m_networkReply; /*!< 用来访问网络请求结果的指针 */
    qint64 m_nReplyErrCode;/*!< 服务器响应错误代码 */
    int m_httpTimeout;  /*!< http超时毫秒 */
    int m_maxRetryTime;  /*!< 网络错误引发的网络命令重试最大次数 */
    int m_curRetryTime /*!< 当前重试次数 */;
    HttpRequestType m_type; /*!< 网络请求类型 */
    QTime m_timeTest;
    QTimer *m_timerTimeout; /*!< 网络请求超时的timer */
    static const int m_defaultHttpTimeout = 30000;  /*!< 默认http超时毫秒 */
    static const int m_defaultMaxRetryTime = 3; /*!< 默认网络错误引发的网络命令重试最大次数 */

private:
    QUuid m_uuid; /*!< http请求的唯一标示码 */
};

#endif // ZREQUESTACTION_H
