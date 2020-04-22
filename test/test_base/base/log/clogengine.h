#ifndef CLOGENGINE_H
#define CLOGENGINE_H
#include <QString>
#include "clogdefines.h"

class CLogEngine
{
public:
    static CLogEngine *getInstance();
    ~CLogEngine();
    void setLogLevel(C_LOG::C_LOG_LEVEL level);
    void setLogHandler(C_LOG::log_handler log_handler);
    void setFieldTypes(C_LOG::FieldTypeFlags fieldTypeFlags);
    void setEncrypted(bool encrypted);

    void outLog(C_LOG::C_LOG_TYPE type,C_LOG::C_LOG_LEVEL level,const QString &lineString,
             const QString &content,const QString &funString,const QString &fileString);

private:
    CLogEngine();
    static QString getThreadField();

private:
    static CLogEngine *m_instance;
    bool m_encrypted;
    C_LOG::C_LOG_TYPE m_type;
    C_LOG::C_LOG_LEVEL m_level;
    C_LOG::FieldTypeFlags m_fieldTypeFlags;
    C_LOG::log_handler m_handler;
};

#endif // CLOGENGINE_H
