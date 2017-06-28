#ifndef ZPUBLIC_H
#define ZPUBLIC_H
#include "zgolbal.h"
#include <QUuid>
#include <QDateTime>

#define Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,key) if(objMap.contains(#key)){ obj.key = objMap.value(#key).toString();}
#define Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,key) if(objMap.contains(#key)){ obj.key = objMap.value(#key).toInt();}

struct Z_WX_COOKIE_PARAM
{
    int ret;
    int isgrayscale;
    QString skey;
    QString wxsid;
    QString wxuin;
    QString pass_ticket;
    Z_WX_COOKIE_PARAM():ret(0),isgrayscale(0){};
};

enum HttpRequestType
{
    TYPE_REQUEST_LOGIN_UUID = 0,
    TYPE_REQUEST_QR_CODE,
    TYPE_REQUEST_WAIT_LOGIN,
    TYPE_REQUEST_COOKIE,
    TYPE_REQUEST_WX_INIT,
    TYPE_REQUEST_AVATAR,
    TYPE_REQUEST_STATUS_NOTIFY,
    TYPE_REQUEST_CONTACT,
    TYPE_REQUEST_SYNC_CHECK,
    TYPE_REQUEST_WX_SYNC,
    TYPE_REQUEST_WX_SEND_MSG,
    TYPE_REQUEST_UNDEFINED
};

struct Z_WX_USER_DATA
{
    QString UserName;
    QString RemarkName;
    QString Signature;
    QString PYInitial;
    QString PYQuanPin;
    QString RemarkPYInitial;
    QString RemarkPYQuanPin;
    QString Province;
    QString City;
    QString Alias;
    QString DisplayName;
    QString EncryChatRoomId;
    QString NickName;
    QString HeadImgUrl;
    int Uin;
    int ContactFlag;
    int MemberCount;
    int HideInputBarFlag;
    int Sex;
    int VerifyFlag;
    int OwnerUin;
    int StarFriend;
    int AppAccountFlag;
    int Statues;
    int AttrStatus;
    int SnsFlag;
    int UniFriend;
    int ChatRoomId;
    int KeyWord;
//    "Uin": 0,
//    "UserName": 用户名称，一个"@"为好友，两个"@"为群组
//    "NickName": 昵称
//    "HeadImgUrl":头像图片链接地址
//    "ContactFlag": 1-好友， 2-群组， 3-公众号
//    "MemberCount": 成员数量，只有在群组信息中才有效,
//    "MemberList": 成员列表,
//    "RemarkName": 备注名称
//    "HideInputBarFlag": 0,
//    "Sex": 性别，0-未设置（公众号、保密），1-男，2-女
//    "Signature": 公众号的功能介绍 or 好友的个性签名
//    "VerifyFlag": 0,
//    "OwnerUin": 0,
//    "PYInitial": 用户名拼音缩写
//    "PYQuanPin": 用户名拼音全拼
//    "RemarkPYInitial":备注拼音缩写
//    "RemarkPYQuanPin": 备注拼音全拼
//    "StarFriend": 是否为星标朋友  0-否  1-是
//    "AppAccountFlag": 0,
//    "Statues": 0,
//    "AttrStatus": 119911,
//    "Province": 省
//    "City": 市
//    "Alias":
//    "SnsFlag": 17,
//    "UniFriend": 0,
//    "DisplayName": "",
//    "ChatRoomId": 0,
//    "KeyWord":
//    "EncryChatRoomId": ""
    QList<Z_WX_USER_DATA> MemberList;
    static Z_WX_USER_DATA parseMap(const QVariantMap &objMap);
};

struct Z_WX_SyncKey_ITEM
{
    int Key;
    int Val;
};

struct Z_WX_SyncKeyList
{
    QList<Z_WX_SyncKey_ITEM> itemList;
    static Z_WX_SyncKeyList parseList(const QVariantList &objList);
};


namespace CPB
{
enum RequestState
{
    REQUEST_STATE_NORMAL,
    REQUEST_STATE_DOING,
    REQUEST_STATE_SUCCESS,
    REQUEST_STATE_FAIL,
};


struct RequestReplyData
{
    RequestReplyData()
        :type(0)
        ,networkErrorCode(-1)
        ,statusCode(-1)
        ,errorValue("")
        ,uuid(QUuid())
        ,state(REQUEST_STATE_NORMAL)
        ,startTime(QDateTime::currentDateTime())
        ,endTime(QDateTime::currentDateTime()){}
    int type;
    int networkErrorCode;
    int statusCode;
    QVariant errorValue;
    QUuid uuid;
    RequestState state;
    QDateTime startTime;
    QDateTime endTime;
    QByteArray replyData;
};
}
class Zpublic
{
public:
    static QList<QStringList> regexCapture(const QString &source,const QString &pattern);
};

#endif // ZPUBLIC_H
