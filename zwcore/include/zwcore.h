#ifndef ZWCORE_H
#define ZWCORE_H

#include "zwcore_global.h"
#include <QString>

class ZWCORESHARED_EXPORT ZWCore
{

public:
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
    static void openFinder(const QString &filePath);
#endif

private:

protected:
    ZWCore();
};

#endif // ZWCORE_H
