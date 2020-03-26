#ifndef ZW3UTILS_H
#define ZW3UTILS_H

#include "zw3utils_global.h"
#include <QString>

class ZW3UTILSSHARED_EXPORT ZW3Utils
{
public:
    ZW3Utils();
    ~ZW3Utils();
    static bool compressedFile(const QStringList &filePathList,const QString &outPath);
};

#endif // ZW3UTILS_H
