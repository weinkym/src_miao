#include "zwydwpublic.h"
#include "zwlog.h"

#define ZW_KEY_DATA(objMap,key,obj,adjuestFun,ok) {\
    if(objMap.contains(#key)){\
    QVariant valueString = objMap.value(#key);\
    obj.key = adjuestFun(valueString,ok);\
    if(!ok){\
    obj.errorString = #key" parse error";\
    return obj;\
    }\
    }\
    else{\
    obj.errorString = #key" not found";\
    return obj;\
    }\
    }

QDate ZWYDWPublic::getAmountDate(const QString &src)
{
    QRegExp re("\\d{4}-\\d{2}-\\d{2}");
    QStringList dateList = getMatchList(src,re);
//    ZW_VALUE_LOG_INFO(dateList.join(";"));
    if(dateList.isEmpty())
    {
        return QDate();
    }
    return QDate::fromString(dateList.first(),"yyyy-MM-dd");
}

QStringList ZWYDWPublic::getMatchList(const QString &src, const QRegExp &re)
{
    int tempPos = 0;
    QStringList tempCapList;
    while ((tempPos = re.indexIn(src, tempPos)) != -1)
    {
        QString cap = re.cap(0);
        tempCapList << cap;
        tempPos += re.matchedLength();
    }
    return tempCapList;
}

double ZWYDWPublic::conertStringToDouble(const QString &src, bool &ok)
{
    QString temp = src;
    temp.remove(",");
    return temp.toDouble(&ok);
}

ZWYDW::MoneyData ZWYDWPublic::parseMoneyData(const QVariantMap &objMap, bool &ok)
{
    ZWYDW::MoneyData obj;

    ZW_KEY_DATA(objMap,amount,obj,ZWYDW::MoneyData::getDoubleDataFun,ok);
    ZW_KEY_DATA(objMap,account_balance,obj,ZWYDW::MoneyData::getDoubleDataFun,ok);
    ZW_KEY_DATA(objMap,recent_receivable_amount,obj,ZWYDW::MoneyData::getDoubleDataFun,ok);
    ZW_KEY_DATA(objMap,recent_refund_amount,obj,ZWYDW::MoneyData::getDoubleDataFun,ok);

    ZW_KEY_DATA(objMap,recent_refund_amount_date,obj,ZWYDW::MoneyData::getDateFun,ok);
    ZW_KEY_DATA(objMap,recent_receivable_amount_date,obj,ZWYDW::MoneyData::getDateFun,ok);

    return obj;

}

ZWYDWPublic::ZWYDWPublic()
{

}

double ZWYDW::MoneyData::getDoubleDataFun(const QVariant &value, bool &ok)
{
    QString str = value.toString();
    if(!str.isEmpty())
    {
        double d = ZWYDWPublic::conertStringToDouble(str,ok);
        if(ok)
        {
            return d;
        }
    }
    else
    {
        ok = false;
    }
    return 0.0;
}

QDate ZWYDW::MoneyData::getDateFun(QVariant &value, bool &ok)
{
    QString str = value.toString();
    if(!str.isEmpty())
    {
        QDate d = ZWYDWPublic::getAmountDate(str);
        if(!d.isNull() && d.isValid())
        {
            ok = true;
            return d;
        }
    }
    ok = false;
    return QDate();
}
