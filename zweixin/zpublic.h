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

struct ZBaseRequestParam
{
    QString skey;
    QString sid;
    QString uin;
    QString deviceID;
    QString pass_ticket;
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
    TYPE_REQUEST_GROUP,
    TYPE_REQUEST_SYNC_CHECK,
    TYPE_REQUEST_WX_SYNC,
    TYPE_REQUEST_WX_SEND_MSG,
    TYPE_REQUEST_UNDEFINED
};
struct Z_WX_MSG_DATA
{
    //
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

enum LOGIN_STATUS
{
    LOGIN_STATUS_ERROR,
    LOGIN_STATUS_LOGINING,
    LOGIN_STATUS_WATING_SCAN,
    LOGIN_STATUS_SCAN_SUCCESSFUL,
    LOGIN_STATUS_REQUEST_COOKIE,
    LOGIN_STATUS_INITIALIZING,
    LOGIN_STATUS_LOGIN_SUCCESSFUL,
};

enum DataUpdateType
{
    DATA_UPDATE_TYPE_CONTACT_LIST,
    DATA_UPDATE_TYPE_GROUP_LIST,
    DATA_UPDATE_TYPE_CONTACT_NAME,
    DATA_UPDATE_TYPE_QR_CODE,
    DATA_UPDATE_TYPE_LOGIN_STATUS,
    DATA_UPDATE_TYPE_ERROR_HAPPEND,
};

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
