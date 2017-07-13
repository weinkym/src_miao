#include "zpublic.h"
#include <QRegExp>
#include <QVariant>
#include <QMap>
#include <QList>
#include <QStandardPaths>
#include <QDir>

QList<QStringList> Zpublic::regexCapture(const QString &source, const QString &pattern)
{
    QRegExp regExp(pattern);
    QList<QStringList> result;
    int pos = 0;
    while((pos = regExp.indexIn(source, pos)) != -1)
    {
        QStringList capList;
        for(int i = 0; i <= regExp.captureCount(); ++i)
        {
            capList.append(regExp.cap(i));
        }
        pos += regExp.matchedLength();
        result.append(capList);
    }
    return result;
}

QString Zpublic::getDataPath(const QString &finderName, bool autoCreatePath)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    if(path.isEmpty())
    {
         path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    path = path+"/" + getApplicationName() + "/"+finderName;
    if(autoCreatePath)
    {
        QDir dir(path);
        dir.mkpath(path);
    }
    return path;
}

QString Zpublic::getApplicationName()
{
    return QString("zweixin");
}

Z_WX_USER_DATA Z_WX_USER_DATA::parseMap(const QVariantMap &objMap)
{
    Z_WX_USER_DATA obj;
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,UserName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,RemarkName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Signature);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,PYInitial);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,PYQuanPin);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,RemarkPYInitial);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,RemarkPYQuanPin);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Province);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,City);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Alias);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,DisplayName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,EncryChatRoomId);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,NickName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,HeadImgUrl);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,Uin);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,ContactFlag);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,MemberCount);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,HideInputBarFlag);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,Sex);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,VerifyFlag);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,OwnerUin);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,StarFriend);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,AppAccountFlag);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,Statues);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,AttrStatus);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,SnsFlag);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,UniFriend);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,ChatRoomId);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,KeyWord);
//    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,MemberList);
    return obj;
}

Z_WX_SyncKeyList Z_WX_SyncKeyList::parseList(const QVariantList &objList)
{
    Z_WX_SyncKeyList syncKeys;
    for(auto d:objList)
    {
        Z_WX_SyncKey_ITEM item;
        QVariantMap objMap = d.toMap();
        Z_DEFINE_PARSE_VALUE_FOR_INT(item,objMap,Key);
        Z_DEFINE_PARSE_VALUE_FOR_INT(item,objMap,Val);
        syncKeys.itemList.append(item);
    }
    return syncKeys;
}

Z_WX_MSG_DATA Z_WX_MSG_DATA::parseMap(const QVariantMap &objMap)
{
    Z_WX_MSG_DATA obj;
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,VoiceLength);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,PlayLength);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,AppMsgType);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,ForwardFlag);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,HasProductId);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,ImgHeight);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,ImgWidth);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,SubMsgType);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,ImgStatus);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,StatusNotifyCode);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,Status);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,MsgType);
    Z_DEFINE_PARSE_VALUE_FOR_LONGLONG(obj,objMap,CreateTime);
    Z_DEFINE_PARSE_VALUE_FOR_LONGLONG(obj,objMap,NewMsgId);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,OriContent);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Content);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,FileName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,FileSize);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,MediaId);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Url);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Ticket);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,ToUserName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,StatusNotifyUserName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,FromUserName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,MsgId);

    obj.AppInfo = Z_WX_AppInfo::parseMap(objMap.value("AppInfo").toMap());
    obj.RecommendInfo = Z_WX_RecommendInfo::parseMap(objMap.value("RecommendInfo").toMap());
    return obj;
}

Z_WX_AppInfo Z_WX_AppInfo::parseMap(const QVariantMap &objMap)
{
    Z_WX_AppInfo obj;
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,Type);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,AppID);
    return obj;
}

Z_WX_RecommendInfo Z_WX_RecommendInfo::parseMap(const QVariantMap &objMap)
{
    Z_WX_RecommendInfo obj;
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,OpCode);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,QQNum);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,Scene);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,VerifyFlag);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,AttrStatus);
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,Sex);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,UserName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,NickName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Province);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,City);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Content);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Signature);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Alias);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,Ticket);
    return obj;
}

CPB::ZEventData::ZEventData()
{
    type = EVENT_TYPE_NODEFINED;
}

QVariantMap CPB::ZEventData::toVariantMap()
{
    QVariantMap objMap;
    Z_DEFINE_INSERT_VARIANTMAP(objMap,type);
    Z_DEFINE_INSERT_VARIANTMAP(objMap,body);
    return objMap;

}

CPB::ZEventData CPB::ZEventData::parseMap(const QVariantMap &objMap)
{
    ZEventData obj;
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,type);
    Z_DEFINE_PARSE_VALUE_FOR_MAP(obj,objMap,body);
    return obj;
}

CPB::AutoSendEventData::AutoSendEventData()
{
     type = AUTO_SEND_EVENT_TYPE_NODEFINED;
     dateTime = 0;
}

QVariantMap CPB::AutoSendEventData::toVariantMap()
{
    QVariantMap objMap;
    Z_DEFINE_INSERT_VARIANTMAP(objMap,type);
    Z_DEFINE_INSERT_VARIANTMAP(objMap,content);
    Z_DEFINE_INSERT_VARIANTMAP(objMap,dateTime);
    Z_DEFINE_INSERT_VARIANTMAP(objMap,toUserName);
    Z_DEFINE_INSERT_VARIANTMAP(objMap,uuid);

    Z_DEFINE_INSERT_VARIANTMAP(objMap,body);
    return objMap;

}

CPB::AutoSendEventData CPB::AutoSendEventData::parseMap(const QVariantMap &objMap)
{
    AutoSendEventData obj;
    Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,type);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,content);

    Z_DEFINE_PARSE_VALUE_FOR_LONGLONG(obj,objMap,dateTime);

    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,toUserName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,uuid);

    Z_DEFINE_PARSE_VALUE_FOR_MAP(obj,objMap,body);

    return obj;
}
