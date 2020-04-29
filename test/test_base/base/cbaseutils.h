#ifndef CBASEUTILS_H
#define CBASEUTILS_H
#include <QString>

class CBaseUtils
{
public:
    CBaseUtils();
    ~CBaseUtils();
    static bool createTempile(const QString &filePath, quint64 size);
};

#endif // CBASEUTILS_H
