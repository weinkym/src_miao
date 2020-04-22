#include "clogfunchelper.h"
#include "clogengine.h"

CLogFuncHelper::CLogFuncHelper(const QString &line, const QString &fun, const QString &file, C_LOG::C_LOG_LEVEL level)
    :m_level(level)
    ,m_lineString(line)
    ,m_funString(fun)
    ,m_fileString(file)
{
    CLogEngine::getInstance()->outLog(C_LOG::TYPE_FUN_RUN,m_level,m_lineString,"ENTER",m_funString,m_fileString);
}

CLogFuncHelper::~CLogFuncHelper()
{
    QString content = QString("LEAVE=%1ms").arg(m_time.msecsTo(QTime::currentTime()));
    CLogEngine::getInstance()->outLog(C_LOG::TYPE_FUN_RUN,m_level,m_lineString,content,m_funString,m_fileString);
}
