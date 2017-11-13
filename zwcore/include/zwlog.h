#ifndef ZWLOG_H
#define ZWLOG_H

#include "zwcore_global.h"

#include <QDebug>
#include <QTime>

enum ZW_LOG_LEVEL
{
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_CRITICAL,
    LOG_LEVEL_FATAL,
};

QString ZWCORESHARED_EXPORT getTestLogContent();
void ZWCORESHARED_EXPORT setTestLogContent(const QString &content);

int ZWCORESHARED_EXPORT getTestLogLevel();
void ZWCORESHARED_EXPORT setTestLogLevel(ZW_LOG_LEVEL level);

int ZWCORESHARED_EXPORT getFunctionLogLevel();
void ZWCORESHARED_EXPORT setFunctionLogLevel(ZW_LOG_LEVEL level);

class ZWCORESHARED_EXPORT ZWLogFuncHelper
{
public:
    ZWLogFuncHelper(const QString &funcionName);
    ~ZWLogFuncHelper();
private:
    QString m_functionInfo;
    QTime m_time;
};

void zw_log(const QString &content,const QString &file,const QString &line,int level);


#define ZW_LOG(X,Y) zw_log(X,QString(__FILE__),QString::number(__LINE__),Y)

#define ZW_LOG_DEBUG(X) ZW_LOG(X,LOG_LEVEL_DEBUG)
#define ZW_LOG_CRITICAL(X) ZW_LOG(X,LOG_LEVEL_CRITICAL)
#define ZW_LOG_FATAL(X) ZW_LOG(X,LOG_LEVEL_FATAL)
#define ZW_LOG_INFO(X) ZW_LOG(X,LOG_LEVEL_INFO)
#define ZW_LOG_WARNING(X) ZW_LOG(X,LOG_LEVEL_WARNING)

#define C_LOG_FUNCTION ZWLogFuncHelper logFuncHelper(Q_FUNC_INFO)

#define ZW_LOG_TEST zw_log(getTestLogContent(),QString(__FILE__),QString::number(__LINE__),getTestLogLevel())

#endif // ZWLOG_H
