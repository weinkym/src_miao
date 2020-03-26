#ifndef ZPUBLIC_H
#define ZPUBLIC_H
#include <QList>
#include <QVariant>
#include <MacTypes.h>
#include "zwlog.h"


class ZPublic
{
public:
    static QList<QPair<QVariant,QVariant > > getCameras();
    static void getVersion(SInt32 &major,SInt32 &minor,SInt32 &bugfix);
    static void testAll();
    static void test();
};

#endif // ZPUBLIC_H
