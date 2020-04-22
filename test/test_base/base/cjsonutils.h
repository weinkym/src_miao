#ifndef CJSONUTILS_H
#define CJSONUTILS_H

#include <QVariant>

#define CJ_VALUE_FROM_MAP(objName, objMap, typeFun) \
    if(objMap.contains(#objName))                   \
    {                                               \
        objName = objMap.value(#objName).typeFun(); \
    }

#define CJ_VALUE_FROM_MAP2(objName, objMap, key, typeFun) \
    if(objMap.contains(#key))                             \
    {                                                     \
        objName = objMap.value(#key).typeFun();           \
    }

#define CJ_OBJ_VALUE_FROM_MAP(obj, objMap, key, typeFun) \
    if(objMap.contains(#key))                            \
    {                                                    \
        obj.key = objMap.value(#key).typeFun();          \
    }

#define CJ_OBJ_VALUE_FROM_MAP2(obj, objMap, objKey, mapKey, typeFun) \
    if(objMap.contains(#objKey))                                     \
    {                                                                \
        obj.objKey = objMap.value(#objKey).typeFun();                \
    }

class CJsonUtils
{
public:
    CJsonUtils();
    ~CJsonUtils();
    static QVariant getVariantMapValue(const QVariantMap &valueMap, const QString &key, bool *ok = 0);

    /*!
     * \brief getVariantMapValueEx 将jsonkey以 "key1.key2.key3"的格式传入，可以用多级key来获取json的值
     * \param valueMap  json data map
     * \param jsonKey  key，分隔符是 “.”
     * \param ok 出参，返回结果，标示成功与否
     * \return 返回的QVariant
     */
    static QVariant getVariantMapValueEx(const QVariantMap &valueMap, const QString &jsonKey, bool *ok = 0);
};

#endif // CJSONUTILS_H
