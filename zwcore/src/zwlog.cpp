#include "zwlog.h"

#include <QThread>

QString g_test_log_content = "TTTT";

int g_test_log_level = LOG_LEVEL_DEBUG;
int g_function_log_level = LOG_LEVEL_DEBUG;

ZWLogFuncHelper::ZWLogFuncHelper(const QString &functionInfo)
{
    m_functionInfo = QString("[function:%1][%2]").arg(functionInfo).arg((long long)QThread::currentThreadId());
    zw_log(QString("[Enter]%1").arg(m_functionInfo),"","",getFunctionLogLevel());
    m_time = QTime::currentTime();
}

ZWLogFuncHelper::~ZWLogFuncHelper()
{
    zw_log(QString("[Leave]%1[%2ms]").arg(m_functionInfo).arg(m_time.msecsTo(QTime::currentTime())),
           "","",getFunctionLogLevel());
}

QString getTestLogContent()
{
    return g_test_log_content;
}

int getTestLogLevel()
{
    return g_test_log_level;
}

int getFunctionLogLevel()
{
    return g_function_log_level;
}

void zw_log(const QString &content, const QString &file, const QString &line, int level)
{
    qDebug()<<(QString("content:")+content+"file:"+file+"line:"+line);
}

void setTestLogContent(const QString &content)
{
    g_test_log_content = content;
}
