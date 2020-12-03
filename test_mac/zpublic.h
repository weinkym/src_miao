#ifndef ZPUBLIC_H
#define ZPUBLIC_H
#include "zwlog.h"
#include <MacTypes.h>
#include <QList>
#include <QVariant>

class ZPublic
{
public:
    static QList<QPair<QVariant, QVariant> > getCameras();
    static void getVersion(SInt32 &major, SInt32 &minor, SInt32 &bugfix);
    static void testAll();
    static void test();
    //    static qint64 getProcessPid(const QString &processName);
    static QList<qreal> getStringNums(const QString &content);
};

#endif // ZPUBLIC_H
