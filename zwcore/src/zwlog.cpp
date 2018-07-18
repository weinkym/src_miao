#include "zwlog.h"

#include <QThread>
#include <QFile>
#include <QMutex>
#include <iostream>

struct ZWLogSettings
{
    ZWLogSettings()
        :test_log_level(LOG_LEVEL_INFO)
        ,function_log_level(LOG_LEVEL_INFO)
        ,current_log_level(LOG_LEVEL_DEBUG)
        ,debug_on(true)
        ,test_log_content("TTTTV")
        ,file_path("")
        ,log_type_separator_begin("[")
        ,log_type_separator_end("]")
        ,types(LOG_TYPE_FILE | LOG_TYPE_DATE_TIME | LOG_TYPE_LINE | LOG_TYPE_FUNCTION | LOG_TYPE_LEVEL_STRING)
    {
    }
    ZW_LOG_LEVEL test_log_level;
    ZW_LOG_LEVEL function_log_level;
    ZW_LOG_LEVEL current_log_level;
    bool debug_on;
    QString test_log_content = "TTTT";
    QString file_path;
    QString log_type_separator_begin;
    QString log_type_separator_end;
    ZW_LOG_TYPES types;
    QFile file;
//    QMutex mutex;
} g_log_settings;

ZWLogFuncHelper::ZWLogFuncHelper(const QString &functionInfo)
{
    m_functionInfo = QString("[function:%1][%2]").arg(functionInfo).arg((long long)QThread::currentThreadId());
    zw_log(QString("[Enter]%1").arg(m_functionInfo),"","","",ZW_LOG::getFunctionLogLevel());
    m_time = QTime::currentTime();
}

ZWLogFuncHelper::~ZWLogFuncHelper()
{
    zw_log(QString("[Leave]%1[%2ms]").arg(m_functionInfo).arg(m_time.msecsTo(QTime::currentTime())),
           "","","",ZW_LOG::getFunctionLogLevel());
}

QString ZW_LOG::getTestLogContent()
{
    return g_log_settings.test_log_content;
}

ZW_LOG_LEVEL ZW_LOG::getTestLogLevel()
{
    return g_log_settings.test_log_level;
}

ZW_LOG_LEVEL ZW_LOG::getFunctionLogLevel()
{
    return g_log_settings.function_log_level;
}

static void addLogTypeString(const QString &value,QString &str)
{
    str.append(g_log_settings.log_type_separator_begin);
    str.append(value);
    str.append(g_log_settings.log_type_separator_end);
}

void zw_log(const QString &content, const QString &funciton, const QString &file, const QString &line, ZW_LOG_LEVEL level)
{
    if(level < g_log_settings.current_log_level)
    {
        return;
    }
    QString str;
    if(g_log_settings.types & LOG_TYPE_DATE_TIME)
    {
        addLogTypeString(QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss"),str);
    }
    addLogTypeString(QString("content:") + content,str);

    if(g_log_settings.types & LOG_TYPE_FILE)
    {
        addLogTypeString(QString("file:") + file,str);
    }
    if(g_log_settings.types & LOG_TYPE_LINE)
    {
        addLogTypeString(QString("line:") + line,str);
    }
    if(g_log_settings.types & LOG_TYPE_LEVEL_STRING)
    {
        addLogTypeString(QString("level:") + ZW_LOG::getLogLevelString(level),str);
    }
    if(g_log_settings.types & LOG_TYPE_FUNCTION)
    {
        addLogTypeString(QString("funciton:") + funciton,str);
    }
    if(g_log_settings.file.isOpen())
    {
        g_log_settings.file.write(str.toLocal8Bit());
        g_log_settings.file.write("\n");
    }
    if(g_log_settings.debug_on)
    {
        qDebug()<<str;
    }
}

void ZW_LOG::setTestLogContent(const QString &content)
{
    g_log_settings.test_log_content = content;
}

void ZW_LOG::setLogTypes(ZW_LOG_TYPES types)
{
    g_log_settings.types = types;
}


void ZW_LOG::initLog(const QString &filePath)
{
//    QMutexLocker locker(&(g_log_settings.mutex));
    if(g_log_settings.file_path == filePath)
    {
        return;
    }
    g_log_settings.file_path = filePath;
    if(g_log_settings.file.isOpen())
    {
        g_log_settings.file.close();
    }
    g_log_settings.file.setFileName(g_log_settings.file_path);
    bool ok = g_log_settings.file.open(QIODevice::WriteOnly);
    if(ok)
    {
        std::cout<<"open log is ok file="<<filePath.toLocal8Bit().data()<<std::endl;
    }
    else
    {
        std::cerr<<"open log is error file="<<filePath.toLocal8Bit().data()<<std::endl;
    }
}

void ZW_LOG::setLogLevel(ZW_LOG_LEVEL level)
{
    g_log_settings.current_log_level = level;
}

ZW_LOG_LEVEL ZW_LOG::getLogLevel()
{
    return g_log_settings.current_log_level;
}

QString ZW_LOG::getLogLevelString(ZW_LOG_LEVEL level)
{
    switch (level)
    {
    case LOG_LEVEL_DEBUG:
        return "LOG_LEVEL_DEBUG";
        break;
    case LOG_LEVEL_INFO:
        return "LOG_LEVEL_INFO";
        break;
    case LOG_LEVEL_WARNING:
        return "LOG_LEVEL_WARNING";
        break;
    case LOG_LEVEL_CRITICAL:
        return "LOG_LEVEL_CRITICAL";
        break;
    case LOG_LEVEL_FATAL:
        return "LOG_LEVEL_FATAL";
        break;
    default:
        break;
    }
   return "unknow log level";
}

void ZW_LOG::setLogTypeSeparator(const QString &separatorBegin, const QString &separatorEnd)
{
    g_log_settings.log_type_separator_begin = separatorBegin;
    g_log_settings.log_type_separator_end = separatorEnd;
}

void ZW_LOG::setDebugStatus(bool on)
{
    g_log_settings.debug_on = on;
}
