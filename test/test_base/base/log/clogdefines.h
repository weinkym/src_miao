#ifndef CLOGDEFINES_H
#define CLOGDEFINES_H

#include <QString>

namespace C_LOG{

typedef void (*log_handler)(int level,const QString &content);

enum C_LOG_TYPE{
    TYPE_NORMAL=1,
    TYPE_API_DATA,
    TYPE_API_ERROR_DATA,
    TYPE_ACTION_LOG,
    TYPE_FUN_RUN,
};

enum C_LOG_LEVEL{
    DEBUG=1,
    INFO,
    WARNING,
    ERROR,
    FATAL,
};

enum FieldType {
    FIELD_TYPE_DATE = 0x00000001,
    FIELD_TYPE_DATETIME = 0x00000002,
    FIELD_TYPE_LINE = 0x00000004,
    FIELD_TYPE_FILE = 0x00000008,
    FIELD_TYPE_LEVEL = 0x00080010,
    FIELD_TYPE_FUN = 0x00080020,
    FIELD_TYPE_THREADID = 0x00080040,
};

Q_DECLARE_FLAGS(FieldTypeFlags, FieldType)
}



#endif // CLOGDEFINES_H
