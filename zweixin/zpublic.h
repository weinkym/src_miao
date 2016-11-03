#ifndef ZPUBLIC_H
#define ZPUBLIC_H
#include "zgolbal.h"

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

struct Z_WX_USER_DATA
{
    QString UserName;
    QString MemberList;
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
};

class Zpublic
{
public:
    static QList<QStringList> regexCapture(const QString &source,const QString &pattern);
    static Z_WX_USER_DATA parseUserData(const QVariantMap &objMap);
};

#endif // ZPUBLIC_H
