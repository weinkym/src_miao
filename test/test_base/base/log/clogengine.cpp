#include "clogengine.h"
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include "clogencryptengine.h"

CLogEngine *CLogEngine::m_instance = NULL;

#define C_LOG_FIELD_DATETIME QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss:zzz: ")
#define C_LOG_FIELD_DATE QDate::currentDate().toString("yyyyMMdd")

#ifdef Q_OS_WIN
#define C_LOG_FIELD_THREAD_ID (QString("[TID=%1]").arg(QString("000000%1").arg((long long)QThread::currentThreadId()).right(6)))
#else
#define C_LOG_FIELD_THREAD_ID (QString("[TID=%1]").arg(QString("00000000000000000000%1").arg((long long)QThread::currentThreadId()).right(16)))
#endif

//todo 有待进一步完善

static void defalutHandler(int level,const QString &content)
{
    qDebug()<<QString("defalutHandler leve=%1,content=%2").arg(level).arg(content);
}

CLogEngine *CLogEngine::getInstance()
{
    //todo有待优化 考虑多线程的情况，并取消设置接口，采用一次性初始化
    if(m_instance == NULL)
    {
        m_instance = new CLogEngine();
    }
    return m_instance;
}

CLogEngine::~CLogEngine()
{

}

void CLogEngine::setLogLevel(C_LOG::C_LOG_LEVEL level)
{
    m_level = level;
}

void CLogEngine::setLogHandler(C_LOG::log_handler log_handler)
{
    m_handler = log_handler;
}

void CLogEngine::setFieldTypes(C_LOG::FieldTypeFlags fieldTypeFlags)
{
    m_fieldTypeFlags = fieldTypeFlags;
}

void CLogEngine::setEncrypted(bool encrypted)
{
    m_encrypted = encrypted;
}

void CLogEngine::outLog(C_LOG::C_LOG_TYPE type, C_LOG::C_LOG_LEVEL level, const QString &lineString, const QString &content, const QString &funString, const QString &fileString)
{
    if(level < m_level || !m_handler)
    {
        return;
    }
    QStringList logList;

    QString new_content = QString::fromLatin1(content.toUtf8().data());
    if(m_fieldTypeFlags & C_LOG::FIELD_TYPE_DATETIME)
    {
        logList.append(C_LOG_FIELD_DATETIME);
    }
    if(m_fieldTypeFlags & C_LOG::FIELD_TYPE_DATE)
    {
        logList.append(C_LOG_FIELD_DATE);
    }
    if(m_fieldTypeFlags & C_LOG::FIELD_TYPE_LEVEL)
    {
        logList.append(QString("[LEVEL=%1]").arg(level));
    }
    if(m_fieldTypeFlags & C_LOG::FIELD_TYPE_THREADID)
    {
        logList.append(getThreadField());
    }
    if(m_fieldTypeFlags & C_LOG::FIELD_TYPE_LINE)
    {
        logList.append(lineString);
    }

    logList.append(QString("[TYPE=%1]").arg(type));
    logList.append(QString("[CONTENT=%1]").arg(m_encrypted ? adjuestContent(new_content) : new_content));
    if(m_fieldTypeFlags & C_LOG::FIELD_TYPE_FUN)
    {
        logList.append(funString);
    }
    if(m_fieldTypeFlags & C_LOG::FIELD_TYPE_FILE)
    {
        logList.append(fileString);
    }
    QString log = logList.join("");
    m_handler(level,log);
}

CLogEngine::CLogEngine()
    : m_encrypted(false)
    , m_type(C_LOG::TYPE_NORMAL)
    , m_level(C_LOG::INFO)
    , m_fieldTypeFlags(C_LOG::FIELD_TYPE_FUN | C_LOG::FIELD_TYPE_DATETIME | C_LOG::FIELD_TYPE_LINE | C_LOG::FIELD_TYPE_FILE | C_LOG::FIELD_TYPE_THREADID)
    , m_handler(defalutHandler)
{

}

QString CLogEngine::getThreadField()
{
#ifdef Q_OS_WIN
 return (QString("[TID=%1]").arg(QString("000000%1").arg((long long)QThread::currentThreadId()).right(6)));
#else
 //return C_LOG_FIELD_THREAD_ID (QString("[TID=%1]").arg(QString("00000000000000000000%1").arg((long long)QThread::currentThreadId()).right(16)));
 return  (QString("[TID=%1]").arg(QString("00000000000000000000%1").arg((long long)QThread::currentThreadId()).right(16)));
#endif
}
