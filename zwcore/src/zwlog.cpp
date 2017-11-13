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
        ,test_log_content("TTTTV")
        ,file_path("")
        ,types(LOG_TYPE_FILE | LOG_TYPE_DATE_TIME | LOG_TYPE_LINE | LOG_TYPE_FUNCTION)
    {
    }
    ZW_LOG_LEVEL test_log_level;
    ZW_LOG_LEVEL function_log_level;
    QString test_log_content = "TTTT";
    QString file_path;
    ZW_LOG_TYPES types;
    QFile file;
    QMutex mutex;
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

void zw_log(const QString &content, const QString &funciton, const QString &file, const QString &line, int level)
{
    QString str = (QString("content:")+content+"funciton:"+funciton+"file:"+file+"line:"+line);
    qDebug()<<str;
    if(g_log_settings.file.isOpen())
    {
        g_log_settings.file.write(str.toLocal8Bit());
        g_log_settings.file.write("\n");
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


void ZW_LOG::setLogFilePath(const QString &filePath)
{
    QMutexLocker locker(&(g_log_settings.mutex));
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
//        printf("open log file = %1\n is ok",filePath.toLocal8Bit().data());
        std::cout<<"open log is ok file="<<filePath.toLocal8Bit().data();
    }
    else
    {
        std::cerr<<"open log is error file="<<filePath.toLocal8Bit().data();
    }
}
