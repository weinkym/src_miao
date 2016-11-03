#include "zpublicaction.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

ZPublicAction *ZPublicAction::createLoginUuidAction()
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_LOGIN_UUID);
    return action;
}

ZPublicAction *ZPublicAction::createQrCodeAction(const QString &uuid)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_QR_CODE);
    action->m_uuid = uuid;
    return action;
}

ZPublicAction *ZPublicAction::createWaitLoginAction(const QString &uuid, int tip)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_WAIT_LOGIN);
    action->m_uuid = uuid;
    action->m_tip = tip;
    return action;
}

ZPublicAction *ZPublicAction::createCookieAction(const QString &uuid, const QString &ticket, const QString &scan)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_COOKIE);
    action->m_uuid = uuid;
    action->m_ticket = ticket;
    action->m_scan = scan;
    return action;
}

ZPublicAction *ZPublicAction::createWxInitAction(const ZBaseRequestParam &baseRequestParam)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_WX_INIT);
    action->m_baseRequestParam = baseRequestParam;
    return action;
}

ZPublicAction *ZPublicAction::createAvatarAction(const QString &url)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_AVATAR);
    action->m_url = url;
    return action;
}

ZPublicAction::ZPublicAction(HttpRequestType type)
    :ZRequestAction(type)
{

}

ZRequestAction::Operation ZPublicAction::getOperation()
{
    switch (m_type)
    {
    case TYPE_REQUEST_LOGIN_UUID:
    case TYPE_REQUEST_WAIT_LOGIN:
    case TYPE_REQUEST_COOKIE:
    case TYPE_REQUEST_AVATAR:
        return Get;
        break;
    case TYPE_REQUEST_QR_CODE:
    case TYPE_REQUEST_WX_INIT:
        return PostByteArray;
        break;
    default:
        break;
    }
    return Get;
}

QNetworkRequest ZPublicAction::getRequest() const
{
    QNetworkRequest request;
    switch (m_type)
    {
    case TYPE_REQUEST_LOGIN_UUID:
    {
        QString head=QString("https://login.weixin.qq.com/jslogin?appid=wx782c26e4c19acffb&fun=new&lang=zh_CN&_=%1").arg(QDateTime::currentDateTime().toTime_t());
//        QNetworkRequest request(QUrl("https://login.weixin.qq.com/jslogin?"
//                                     "appid=wx782c26e4c19acffb&redirect_uri=https%3A%2F%2Fwx.qq.com%2Fcgi-bin%2Fmmwebwx-bin%2Fwebwxnewloginpage"
//                                     "&fun=new&lang=zh_CN&_={t}"));

        QNetworkRequest request(QUrl(head.toLatin1()));
        return request;
        break;
    }
    case TYPE_REQUEST_QR_CODE:
    {
        QString head=QString("https://login.weixin.qq.com/qrcode/%1").arg(m_uuid);

        QNetworkRequest request(QUrl(head.toLatin1()));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

        return request;
        break;
    }
    case TYPE_REQUEST_WAIT_LOGIN:
    {
        QString head=QString("https://login.weixin.qq.com/cgi-bin/mmwebwx-bin/login?uuid=%1&tip=%2&_=%3")
                .arg(m_uuid).arg(m_tip).arg(QDateTime::currentDateTime().toTime_t());

        QNetworkRequest request(QUrl(head.toLatin1()));
        return request;
        break;
    }
    case TYPE_REQUEST_COOKIE:
    {
        QString head=QString("https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxnewloginpage?ticket=%1&uuid=%2&lang=zh_CN&scan=%3&fun=new")
                .arg(m_ticket).arg(m_uuid).arg(m_scan);

        QNetworkRequest request(QUrl(head.toLatin1()));
        return request;
        break;
    }
    case TYPE_REQUEST_WX_INIT:
    {
        QString head=QString("https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxinit?r=%1&pass_ticket=%2").arg(QDateTime::currentDateTime().toTime_t()).arg(m_baseRequestParam.pass_ticket);

        QNetworkRequest request(QUrl(head.toLatin1()));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"Content-Type: application/json; charset=UTF-8");
        return request;
        break;
    }
    case TYPE_REQUEST_AVATAR:
    {
        QString head=QString("https://wx.qq.com%1").arg(m_url);
        QNetworkRequest request(QUrl(head.toLatin1()));
        return request;
        break;
    }

    default:
        break;
    }
    return QNetworkRequest();
}

QByteArray ZPublicAction::getByteArray() const
{
    QByteArray array;
    switch(m_type)
    {
    case TYPE_REQUEST_QR_CODE:
    {
//        QByteArray array;
//        QJsonObject payloadObj;
//        QJsonObject chatObj;

//        chatObj.insert(JK_CONTENT, m_content);
//        payloadObj.insert(JK_CHAT, chatObj);

        QJsonObject obj;
        obj.insert("t","webwx");
        obj.insert("_",QJsonValue::fromVariant(QDateTime::currentDateTime().toTime_t()));
//        QJsonValue destIdValue = QJsonValue::fromVariant(m_destId);
//        obj.insert(JK_DEST_ID, destIdValue);
//        QJsonValue destTypeValue = QJsonValue::fromVariant(m_destType);
//        obj.insert(JK_DEST_TYPE, destTypeValue);
//        QJsonValue typeValue = QJsonValue::fromVariant(m_msgType);
//        obj.insert(JK_TYPE, typeValue);
//        obj.insert(JK_BODY, payloadObj);

        QJsonDocument objDoc(obj);
        array = objDoc.toJson();
        break;
    }
    case TYPE_REQUEST_WX_INIT:
    {
        QJsonObject obj;
        QJsonObject objBaseRequest;
        objBaseRequest.insert("Uin",m_baseRequestParam.uin);
        objBaseRequest.insert("Sid",m_baseRequestParam.sid);
        objBaseRequest.insert("Skey",m_baseRequestParam.skey);
        objBaseRequest.insert("DeviceID",m_baseRequestParam.deviceID);
        obj.insert("BaseRequest", objBaseRequest);
        QJsonDocument objDoc(obj);
        array = objDoc.toJson();
        break;
    }
    default:
        break;
    }

    return array;
}
