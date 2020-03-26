#ifndef ZWLOG_H
#define ZWLOG_H

#include "zwcore_global.h"

#include <QDebug>
#include <QTime>
#include <QFlags>
#include "zwlogvalueengine.h"

enum ZW_LOG_LEVEL
{
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_CRITICAL,
    LOG_LEVEL_FATAL,
};

enum ZW_LOG_TYPE
{
    LOG_TYPE_FILE = 0x00000001,
    LOG_TYPE_DATE = LOG_TYPE_FILE<<1,
    LOG_TYPE_DATE_TIME = LOG_TYPE_DATE<<1,
    LOG_TYPE_DATE_TIMES = LOG_TYPE_DATE_TIME<<1,
    LOG_TYPE_THREAD = LOG_TYPE_DATE_TIMES<<1,
    LOG_TYPE_LINE = LOG_TYPE_THREAD<<1,
    LOG_TYPE_FUNCTION = LOG_TYPE_LINE<<1,
    LOG_TYPE_LEVEL_STRING = LOG_TYPE_FUNCTION<<1,
};
Q_DECLARE_FLAGS(ZW_LOG_TYPES, ZW_LOG_TYPE)

namespace ZW_LOG
{
QString ZWCORESHARED_EXPORT getTestLogContent();
void ZWCORESHARED_EXPORT setTestLogContent(const QString &content);

ZW_LOG_LEVEL ZWCORESHARED_EXPORT getTestLogLevel();
void ZWCORESHARED_EXPORT setTestLogLevel(ZW_LOG_LEVEL level);

ZW_LOG_LEVEL ZWCORESHARED_EXPORT getFunctionLogLevel();
void ZWCORESHARED_EXPORT setFunctionLogLevel(ZW_LOG_LEVEL level);

ZW_LOG_LEVEL ZWCORESHARED_EXPORT getLogLevel();
void ZWCORESHARED_EXPORT setLogLevel(ZW_LOG_LEVEL level);

void ZWCORESHARED_EXPORT setLogTypes(ZW_LOG_TYPES types);

void ZWCORESHARED_EXPORT initLog(const QString &filePath);

QString ZWCORESHARED_EXPORT getLogLevelString(ZW_LOG_LEVEL level);

void ZWCORESHARED_EXPORT setLogTypeSeparator(const QString &separatorBegin,const QString &separatorEnd);

void ZWCORESHARED_EXPORT setDebugStatus(bool on);
}

void ZWCORESHARED_EXPORT zw_log(const QString &content,const QString &funciton,const QString &file,const QString &line,ZW_LOG_LEVEL level);

class ZWCORESHARED_EXPORT ZWLogFuncHelper
{
public:
    ZWLogFuncHelper(const QString &funcionName);
    ~ZWLogFuncHelper();
private:
    QString m_functionInfo;
    QTime m_time;
};

#define ZW_LOG_P(value) (QString(#value"=%1").arg(createLogContent(value)))
#define ZW_LOG_P2(value1,value2) (ZW_LOG_P(value1)+","+ZW_LOG_P(value2))
#define ZW_LOG_P3(value1,value2,value3) (ZW_LOG_P2(value1,value2)+","+ZW_LOG_P(value3))
#define ZW_LOG_P4(value1,value2,value3,value4) (ZW_LOG_P2(value1,value2)+","+ZW_LOG_P2(value3,value4))
#define ZW_LOG_P5(value1,value2,value3,value4,value5) (ZW_LOG_P3(value1,value2,value3)+","+ZW_LOG_P2(value4,value5))
#define ZW_LOG_P6(value1,value2,value3,value4,value5,value6) (ZW_LOG_P3(value1,value2,value3)+","+ZW_LOG_P3(value4,value5,value6))

#define ZW_LOG(X,LEVEL) zw_log(X,QString(Q_FUNC_INFO),QString(__FILE__),QString::number(__LINE__),LEVEL)

#define ZW_LOG_FUNCTION ZWLogFuncHelper logFuncHelper(Q_FUNC_INFO)

#define ZW_LOG_DEBUG(X) ZW_LOG(X,LOG_LEVEL_DEBUG)
#define ZW_LOG_CRITICAL(X) ZW_LOG(X,LOG_LEVEL_CRITICAL)
#define ZW_LOG_FATAL(X) ZW_LOG(X,LOG_LEVEL_FATAL)
#define ZW_LOG_INFO(X) ZW_LOG(X,LOG_LEVEL_INFO)
#define ZW_LOG_WARNING(X) ZW_LOG(X,LOG_LEVEL_WARNING)

#define ZW_LOG_TEST ZW_LOG(ZW_LOG_P2(__LINE__,__FILE__),LOG_LEVEL_DEBUG)


#endif // ZWLOG_H
