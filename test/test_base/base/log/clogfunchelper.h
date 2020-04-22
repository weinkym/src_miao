#ifndef CLOGFUNCHELPER_H
#define CLOGFUNCHELPER_H

#include <QString>
#include <QTime>
#include "clogdefines.h"

class CLogFuncHelper
{
public:
    CLogFuncHelper(const QString &line,const QString &fun,const QString &file,C_LOG::C_LOG_LEVEL level);
    ~CLogFuncHelper();
private:
    C_LOG::C_LOG_LEVEL m_level;
    QString m_lineString;
    QString m_funString;
    QString m_fileString;
    QString m_enterLog;
    QString m_leaveLog;
    QTime m_time;
};

#endif // CLOGFUNCHELPER_H
