#ifndef ZPUBLIC_H
#define ZPUBLIC_H
#include "zgolbal.h"
#include <QUuid>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlDatabase>

#define Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,key) if(objMap.contains(#key)){ obj.key = objMap.value(#key).toString();}
#define Z_DEFINE_PARSE_VALUE_FOR_INT(obj,objMap,key) if(objMap.contains(#key)){ obj.key = objMap.value(#key).toInt();}
#define Z_DEFINE_PARSE_VALUE_FOR_LONGLONG(obj,objMap,key) if(objMap.contains(#key)){ obj.key = objMap.value(#key).toLongLong();}
#define Z_DEFINE_PARSE_VALUE_FOR_MAP(obj,objMap,key) if(objMap.contains(#key)){ obj.key = objMap.value(#key).toMap();}


#define Z_DEFINE_INSERT_VARIANTMAP(objMap,key) { objMap.insert((#key),key);}

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

struct Z_WX_AppInfo
{
    int Type;
    QString AppID;
    static Z_WX_AppInfo parseMap(const QVariantMap &objMap);
};

struct Z_WX_RecommendInfo
{
    int OpCode;
    int QQNum;
    int Scene;
    int VerifyFlag;
    int AttrStatus;
    int Sex;
    QString UserName;
    QString NickName;
    QString Province;
    QString City;
    QString Content;
    QString Signature;
    QString Alias;
    QString Ticket;
    static Z_WX_RecommendInfo parseMap(const QVariantMap &objMap);
};

struct Z_WX_MSG_DATA
{
    int VoiceLength;
    int PlayLength;
    int AppMsgType;
    int ForwardFlag;
    int HasProductId;
    int ImgHeight;
    int ImgWidth;
    int SubMsgType;
    int ImgStatus;
    int StatusNotifyCode;
    int Status;
    int MsgType;
    qlonglong CreateTime;
    qlonglong NewMsgId;
    QString OriContent;
    QString Content;
    QString FileName;
    QString FileSize;
    QString MediaId;
    QString Url;
    QString Ticket;
    QString ToUserName;
    QString StatusNotifyUserName;
    QString FromUserName;
    QString MsgId;

    Z_WX_AppInfo AppInfo;
    Z_WX_RecommendInfo RecommendInfo;
    static Z_WX_MSG_DATA parseMap(const QVariantMap &objMap);
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

enum EventType
{
    EVENT_TYPE_NODEFINED,
    EVENT_TYPE_AUTO_SEND,
};

struct ZEventData
{
    int type;
    QVariantMap body;
    ZEventData();
    QVariantMap toVariantMap();
    static ZEventData parseMap(const QVariantMap &objMap);
};

enum AutoSendEventType
{
    AUTO_SEND_EVENT_TYPE_NODEFINED,
    AUTO_SEND_EVENT_TYPE_MONTH,
    AUTO_SEND_EVENT_TYPE_DAY,
    AUTO_SEND_EVENT_TYPE_HOUR,
    AUTO_SEND_EVENT_TYPE_NORMAL,
};

enum FieldType
{
    TypeInteger,
    TypeString,
    TypeDouble
};

struct Field
{
    QString keyInDb;
    FieldType type;
};

struct BindValueParam
{
    BindValueParam():paramType(QSql::In){};
    QSql::ParamType paramType;
    QString prepareString;
    QMap<QString,QVariant> bindValueMap;
};

struct AutoSendEventData
{
    int type;
    qlonglong dateTime;
    QString content;
    QString toUserName;
    QString uuid;
    QString body;
    AutoSendEventData();
    QVariantMap toVariantMap() const;
    static AutoSendEventData parseMap(const QVariantMap &objMap);
    static QMap<QString, Field> getFieldMap();
};
}
class Zpublic
{
public:
    static QList<QStringList> regexCapture(const QString &source,const QString &pattern);
    static QString getDataPath(const QString &finderName,bool autoCreatePath);
    static QString getApplicationName();
    static QString getMessageInsertSql(const CPB::AutoSendEventData &msg);
};

#endif // ZPUBLIC_H
