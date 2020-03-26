#ifndef ZREQUESTACCESSMANAGER_H
#define ZREQUESTACCESSMANAGER_H

#include <QObject>
#include <QObject>
#include <QNetworkAccessManager>
#include <QBuffer>
#include "zrequestaction.h"
#include "zgolbal.h"

/*! \brief 实现了NetworkAccess类,用于网络模块动作的触发和响应
 *类主要用于从RequestAction中获取网络通信信息并使用Http协议发送/接收
 */
class  ZRequestAccessManager : public QObject
{
    Q_OBJECT
public:
    /*! \brief 静态函数,用于NetworkAccess单例的实现
     *  \return 返回NetworkAccess的唯一实例
     */
    static ZRequestAccessManager *getInstance();

    /*! \brief 获取NetworkAccess使用的QNetworkAccessManager
     *  \return 返回唯一实例使用的QNetworkAccessManager
     */
    QNetworkAccessManager* getAccessMgr() { return m_access; }

    /*! \brief 设置cookie
     *  \param cookieList 待设置的cookie list
     *  \param domain cookie相关联的domain
     *  \return ture, 设置cookie生效； false， 设置cookie失效
     */
    bool setCookie(const QVariant &cookieListVariant, const QString &domain);

    /*! \brief 更新yxt token
     */
    void refreshYxtToken();

    /*! \brief 设置登录状态
     */
    void setYxtLoginStatus(bool isLogging);

    /*! \brief 使用Http协议post发送文本数据
     *  \param action 网络通信动作
     */
    void postByteArray(ZRequestAction *action);

    void putByteArray(ZRequestAction *action);

    void postWithttachment(ZRequestAction *action);

    /*! \brief 使用Http协议get请求数据
     *  \param action 网络通信动作
     */
    void get(ZRequestAction *action);

    /*! \brief 使用Http协议patch更新文本数据
     *  \param action 网络通信动作
     */
    void patchByteArray(ZRequestAction *action);

    /*! \brief 使用Http协议patch更新多媒体数据
     *  \param action 网络通信动作
     */
    void patchMulti(ZRequestAction *action);

    void deleteResource(ZRequestAction *action);

    /*! \brief 重置网络访问状态。解决QNetworkAccessManager 的bug。bug 被fix 之后可以去掉此函数;
     */
    void recoverNetworkAccessible();

    /*! \brief 获取网络连接状态
     *  \return true 网络可用, false 网络不可用
     */
    bool getNetworkAccessible();

signals:
    /*! \brief 登录信息改变信号,当令牌,场景,推送服务器改变时发送
     *  \param strToken 新的令牌字符串
     *  \param strSession 新的场景字符串
     *  \param strEndPoint 新的推送服务器字符串
     */
    void sigCookieChanged(const QString &strToken, const QString &strSession, const QString &strEndPoint);

    /*! \brief 云学堂token改变时发信号
     */
    void sigYxtTokenChanged();

    /*! \brief 信号通知网络访问状态的改变
     *  \param networkAccessibility true 网络可访问，false 网络不可访问
     */
    void sigUpdateNetworkAccessibility(bool networkAccessibility);


private slots:
    /*! \brief 私有槽函数，忽略ssl错误
     *  \param reply network reply
     *  \param sslErrors 返回的ssl 错误
     */
    void onIgnoreSSlErrors(QNetworkReply* reply, QList<QSslError> sslErrors);

    /*! \brief 私有槽函数，监控网络状态
     *  \param status 网络可用状态
     */
    void onNetworkAccessibilityChanged(QNetworkAccessManager::NetworkAccessibility status);

    /*! \brief 私有槽函数，网络请求结束回收资源
     *  \param reply 请求对应的reply
     */
    void onRequestFinished(QNetworkReply *reply);


protected:
    /*! \brief 构造函数
     *  \param parent 父对象指针
     */
    explicit ZRequestAccessManager(QObject *parent = 0);

private:
    QNetworkAccessManager *m_access;/*!< 网络通信管理指针 */
    static ZRequestAccessManager *m_instance;/*!< 唯一实例*/

    // !TODO 目前程序的启动机制还未修改，作一个临时解决方案
    bool m_isYxtLogining;
};


#endif // ZREQUESTACCESSMANAGER_H
