#ifndef ZWLOG_H
#define ZWLOG_H

#include "zwcore_global.h"

#include <QDebug>
#include <QTime>
#include <QFlags>

enum ZW_LOG_LEVEL
{
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
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

void ZWCORESHARED_EXPORT setLogTypes(ZW_LOG_TYPES types);

void ZWCORESHARED_EXPORT setLogFilePath(const QString &filePath);
void ZWCORESHARED_EXPORT initLog();
void ZWCORESHARED_EXPORT closeLog();
}

void zw_log(const QString &content,const QString &funciton,const QString &file,const QString &line,int level);

class ZWCORESHARED_EXPORT ZWLogFuncHelper
{
public:
    ZWLogFuncHelper(const QString &funcionName);
    ~ZWLogFuncHelper();
private:
    QString m_functionInfo;
    QTime m_time;
};



#define ZW_LOG(X,Y) zw_log(X,QString(Q_FUNC_INFO),QString(__FILE__),QString::number(__LINE__),Y)

#define ZW_LOG_DEBUG(X) ZW_LOG(X,LOG_LEVEL_DEBUG)
#define ZW_LOG_CRITICAL(X) ZW_LOG(X,LOG_LEVEL_CRITICAL)
#define ZW_LOG_FATAL(X) ZW_LOG(X,LOG_LEVEL_FATAL)
#define ZW_LOG_INFO(X) ZW_LOG(X,LOG_LEVEL_INFO)
#define ZW_LOG_WARNING(X) ZW_LOG(X,LOG_LEVEL_WARNING)

#define C_LOG_FUNCTION ZWLogFuncHelper logFuncHelper(Q_FUNC_INFO)

#define ZW_LOG_TEST zw_log(getTestLogContent(),QString(__FILE__),QString::number(__LINE__),getTestLogLevel())

#endif // ZWLOG_H
