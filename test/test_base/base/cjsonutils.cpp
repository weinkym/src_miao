#include "cjsonutils.h"
#include "clogsetting.h"

CJsonUtils::CJsonUtils()
{
}

CJsonUtils::~CJsonUtils()
{
}

QVariant CJsonUtils::getVariantMapValue(const QVariantMap &valueMap, const QString &key, bool *ok)
{
    C_LOG_FUNCTION_DEBUG;
    QVariant value;

    if(valueMap.contains(key))
    {
        value = valueMap.value(key);
        if(ok)
        {
            *ok = true;
        }
    }
    else
    {
        if(ok)
        {
            *ok = false;
        }
        C_LOG_WARNING(QString("json doesn't has the key(%1),json keys(%2)")
                          .arg(key)
                          .arg(QStringList(valueMap.keys()).join(";")));
    }
    return value;
}

QVariant CJsonUtils::getVariantMapValueEx(const QVariantMap &valueMap, const QString &jsonKey, bool *ok)
{
    C_LOG_FUNCTION_DEBUG;
    QStringList keyList = jsonKey.split(".");
    QVariant objData;
    QVariantMap tempMap = valueMap;
    bool isOk = false;
    for(int i = 0; i < keyList.count(); ++i)
    {
        QString key = keyList.at(i);
        if(tempMap.contains(key))
        {
            if(i == (keyList.count() - 1))
            {
                objData = tempMap.value(key);
                isOk = true;
                break;
            }
            else
            {
                tempMap = tempMap.value(key).toMap();
            }
        }
        else
        {
            break;
        }
    }
    if(ok)
    {
        *ok = isOk;
    }
    if(!isOk)
    {
        C_LOG_WARNING(QString("data found failure,%1").arg(C_LOG_P(jsonKey)));
    }
    return objData;
}
