#include "zpublic.h"
#include <QRegExp>

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

Z_WX_USER_DATA Zpublic::parseUserData(const QVariantMap &objMap)
{
    Z_WX_USER_DATA obj;
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,UserName);
    Z_DEFINE_PARSE_VALUE_FOR_STRING(obj,objMap,MemberList);
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
    return obj;
}
