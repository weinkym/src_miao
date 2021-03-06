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

ZPublicAction *ZPublicAction::createCookieRedirecturlAction(const QString &redirecturl)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_COOKIE_REDIRECTURL);
    action->m_redirecturl = redirecturl;
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

ZPublicAction *ZPublicAction::createStatusNotify(const ZBaseRequestParam &baseRequestParam, const QString &fromUserName, const QString &toUserName)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_STATUS_NOTIFY);
    action->m_baseRequestParam = baseRequestParam;
    action->m_fromUserName = fromUserName;
    action->m_toUserName = toUserName;
    return action;
}

ZPublicAction *ZPublicAction::createGetContact(const ZBaseRequestParam &baseRequestParam)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_CONTACT);
    action->m_baseRequestParam = baseRequestParam;
    return action;
}

ZPublicAction *ZPublicAction::createGetGroup(const ZBaseRequestParam &baseRequestParam, const QStringList &groupNameList)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_GROUP);
    action->m_baseRequestParam = baseRequestParam;
    action->m_groupNameList = groupNameList;
    return action;
}

ZPublicAction *ZPublicAction::createSendMessage(const ZBaseRequestParam &baseRequestParam, const QString &fromUserName,
                                                const QString &toUserName, const QString &message)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_WX_SEND_MSG);
    action->m_baseRequestParam = baseRequestParam;
    action->m_fromUserName = fromUserName;
    action->m_toUserName = toUserName;
    action->m_message = message;
    return action;
}

ZPublicAction *ZPublicAction::createSyncCheck(const ZBaseRequestParam &baseRequestParam, const Z_WX_SyncKeyList &syncKeyList)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_SYNC_CHECK);
    action->m_baseRequestParam = baseRequestParam;
    action->m_syncKeyList = syncKeyList;
    return action;
}

ZPublicAction *ZPublicAction::createWXSync(const ZBaseRequestParam &baseRequestParam, const Z_WX_SyncKeyList &syncKeyList)
{
    ZPublicAction *action = new ZPublicAction(TYPE_REQUEST_WX_SYNC);
    action->m_baseRequestParam = baseRequestParam;
    action->m_syncKeyList = syncKeyList;
    return action;
}

ZPublicAction::ZPublicAction(HttpRequestType type)
    :ZRequestAction(type)
{

}

ZRequestAction::Operation ZPublicAction::getOperation()
{
    switch (getType())
    {
    case TYPE_REQUEST_LOGIN_UUID:
    case TYPE_REQUEST_WAIT_LOGIN:
    case TYPE_REQUEST_COOKIE:
    case TYPE_REQUEST_COOKIE_REDIRECTURL:
    case TYPE_REQUEST_AVATAR:
    case TYPE_REQUEST_CONTACT:
    case TYPE_REQUEST_SYNC_CHECK:
        return Get;
        break;
    case TYPE_REQUEST_QR_CODE:
    case TYPE_REQUEST_WX_INIT:
    case TYPE_REQUEST_STATUS_NOTIFY:
    case TYPE_REQUEST_WX_SEND_MSG:
    case TYPE_REQUEST_GROUP:
    case TYPE_REQUEST_WX_SYNC:
        return PostByteArray;
        break;
    default:
        break;
    }
    return Get;
}

QNetworkRequest ZPublicAction::createRequest() const
{
    QNetworkRequest request;
    switch (getType())
    {
    case TYPE_REQUEST_LOGIN_UUID:
    {
        QString head=QString("https://login.weixin.qq.com/jslogin?appid=wx782c26e4c19acffb&redirect_uri:https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxnewloginpage&fun=new&lang=zh_CN&_=%1")
                .arg(QDateTime::currentDateTime().toTime_t());
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
        QString head=QString("https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxnewloginpage?ticket=%1&uuid=%2&lang=zh_CN&scan=%3&fun=new&version=v2")
                .arg(m_ticket).arg(m_uuid).arg(m_scan);

        QNetworkRequest request(QUrl(head.toLatin1()));
        return request;
        break;
    }
    case TYPE_REQUEST_COOKIE_REDIRECTURL:
    {
        QNetworkRequest request(QUrl(m_redirecturl.toLatin1()));
        return request;
        break;
    }
    case TYPE_REQUEST_WX_INIT:
    {
        QString head=QString("https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxinit?r=-%1&lang=zh_CN&pass_ticket=%2").arg(QDateTime::currentDateTime().toTime_t()).arg(m_baseRequestParam.pass_ticket);

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
    case TYPE_REQUEST_STATUS_NOTIFY:
    {
        QString head=QString("https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxstatusnotify?lang=zh_CN&pass_ticket=%1").arg(m_baseRequestParam.pass_ticket);

        QNetworkRequest request(QUrl(head.toLatin1()));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"Content-Type: application/json; charset=UTF-8");
        return request;
        break;
    }
    case TYPE_REQUEST_CONTACT:
    {
        QString head=QString("https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxgetcontact?lang=zh_CN&pass_ticket=%1&seq=0&skey=%2&r=%3")
                .arg(m_baseRequestParam.pass_ticket).arg(m_baseRequestParam.skey).arg(QDateTime::currentDateTime().toTime_t());
        QNetworkRequest request(QUrl(head.toLatin1()));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"Content-Type: application/json; charset=UTF-8");
        return request;
        break;
    }
    case TYPE_REQUEST_GROUP:
    {
        QString head=QString("https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxbatchgetcontact?type=ex&pass_ticket=%1&r=%2&lang=zh_CN")
                .arg(m_baseRequestParam.pass_ticket).arg(QDateTime::currentDateTime().toTime_t());
        QNetworkRequest request(QUrl(head.toLatin1()));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"Content-Type: application/json; charset=UTF-8");
        return request;
        break;
    }
    case TYPE_REQUEST_WX_SEND_MSG:
    {
        QString head=QString("https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxsendmsg?lang=zh_CN&pass_ticket=%1")
                .arg(m_baseRequestParam.pass_ticket);
        QNetworkRequest request(QUrl(head.toLatin1()));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"Content-Type: application/json; charset=UTF-8");
        return request;
        break;
    }
    case TYPE_REQUEST_SYNC_CHECK:
    {
        QDateTime dateTime = QDateTime::currentDateTime();
        qlonglong r = dateTime.toTime_t() * (qlonglong)1000 + dateTime.time().msec();
        static qlonglong g_synckey_key = 0;
        static bool isFirst = true;
        if(isFirst)
        {
            isFirst = false;
            g_synckey_key = r - 5000;
        }
        else
        {
            g_synckey_key += 1;
        }
       QStringList synckeyList;
       for(auto obj:m_syncKeyList.itemList)
       {
           synckeyList.append(QString("%1_%2").arg(obj.Key).arg(obj.Val));
       }
       QString synckey = synckeyList.join("|");
        QString head=QString("https://webpush.wx.qq.com/cgi-bin/mmwebwx-bin/synccheck?r=%1&skey=%2&sid=%3&uin=%4&deviceid=%5&synckey=%6&_=%7")
                .arg(r).arg(m_baseRequestParam.skey).arg(m_baseRequestParam.sid).arg(m_baseRequestParam.uin).arg(m_baseRequestParam.deviceID)
                .arg(synckey).arg(g_synckey_key);
        QNetworkRequest request(QUrl(head.toLatin1()));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"Content-Type: application/json; charset=UTF-8");
        return request;
        break;
    }
    case TYPE_REQUEST_WX_SYNC:
    {
        QString head=QString("https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxsync?sid=%1&skey=%2&lang=zh_CN&pass_ticket=%3")
                .arg(m_baseRequestParam.sid).arg(m_baseRequestParam.skey).arg(m_baseRequestParam.pass_ticket);
        QNetworkRequest request(QUrl(head.toLatin1()));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"Content-Type: application/json; charset=UTF-8");
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
    switch(getType())
    {
    case TYPE_REQUEST_QR_CODE:
    {
        QJsonObject obj;
        obj.insert("t","webwx");
        obj.insert("_",QJsonValue::fromVariant(QDateTime::currentDateTime().toTime_t()));
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
    case TYPE_REQUEST_STATUS_NOTIFY:
    {
        QJsonObject obj;
        QJsonObject objBaseRequest;
        objBaseRequest.insert("Uin",m_baseRequestParam.uin);
        objBaseRequest.insert("Sid",m_baseRequestParam.sid);
        objBaseRequest.insert("Skey",m_baseRequestParam.skey);
        objBaseRequest.insert("DeviceID",m_baseRequestParam.deviceID);
        obj.insert("BaseRequest", objBaseRequest);
        obj.insert("Code",3);
        obj.insert("FromUserName",m_fromUserName);
        obj.insert("ToUserName",m_toUserName);
        obj.insert("ClientMsgId",QJsonValue::fromVariant(QDateTime::currentDateTime().toTime_t()));

        QJsonDocument objDoc(obj);
        array = objDoc.toJson();
        break;
    }
    case TYPE_REQUEST_GROUP:
    {
        QJsonObject obj;
        QJsonObject objBaseRequest;
        objBaseRequest.insert("Uin",m_baseRequestParam.uin);
        objBaseRequest.insert("Sid",m_baseRequestParam.sid);
        objBaseRequest.insert("Skey",m_baseRequestParam.skey);
        objBaseRequest.insert("DeviceID",m_baseRequestParam.deviceID);
        obj.insert("BaseRequest", objBaseRequest);

        QJsonArray jsonArray;
        for(auto d:m_groupNameList)
        {
            QJsonObject tempObj;
            tempObj.insert("UserName",d);
            tempObj.insert("EncryChatRoomId","");
            jsonArray.append(tempObj);
        }
        obj.insert("List",jsonArray);
        obj.insert("Count",m_groupNameList.count());
        QJsonDocument objDoc(obj);
        array = objDoc.toJson();
        break;
    }
    case TYPE_REQUEST_WX_SEND_MSG:
    {
        QJsonObject obj;
        QJsonObject objBaseRequest;
        objBaseRequest.insert("Uin",m_baseRequestParam.uin);
        objBaseRequest.insert("Sid",m_baseRequestParam.sid);
        objBaseRequest.insert("Skey",m_baseRequestParam.skey);
        objBaseRequest.insert("DeviceID",m_baseRequestParam.deviceID);
        obj.insert("BaseRequest", objBaseRequest);

        QJsonObject objMsg;

        quint64 id = QDateTime::currentDateTime().toTime_t();
        id<<4;
        id += qrand() %10000;
        ZW_LOG_DEBUG(QString("client id = %1").arg(id));

        objMsg.insert("ClientMsgId",QJsonValue::fromVariant(id));
        objMsg.insert("Content",m_message);
        objMsg.insert("LocalID",QJsonValue::fromVariant(id));
        objMsg.insert("FromUserName",m_fromUserName);
        objMsg.insert("ToUserName",m_toUserName);
        objMsg.insert("Type",1);

        obj.insert("Msg", objMsg);
        obj.insert("Scene",0);

        QJsonDocument objDoc(obj);
        array = objDoc.toJson();
        break;
    }
    case TYPE_REQUEST_WX_SYNC:
    {
        QJsonObject obj;
        QJsonObject objBaseRequest;
        objBaseRequest.insert("Uin",m_baseRequestParam.uin);
        objBaseRequest.insert("Sid",m_baseRequestParam.sid);
        objBaseRequest.insert("Skey",m_baseRequestParam.skey);
        objBaseRequest.insert("DeviceID",m_baseRequestParam.deviceID);
        obj.insert("BaseRequest", objBaseRequest);

        QJsonObject objSyncKey;
        objSyncKey.insert("Count",m_syncKeyList.itemList.count());

        QJsonArray jsonArray;
        for(auto d:m_syncKeyList.itemList)
        {
            QJsonObject tempObj;
            tempObj.insert("Key",d.Key);
            tempObj.insert("Val",d.Val);
            jsonArray.append(tempObj);
        }
        objSyncKey.insert("List",jsonArray);
        obj.insert("SyncKey", objSyncKey);

        int timet = QDateTime::currentDateTime().toTime_t();
        obj.insert("rr", ~(timet));

        QJsonDocument objDoc(obj);
        array = objDoc.toJson();
        break;
    }
    default:
        break;
    }

    return array;
}
