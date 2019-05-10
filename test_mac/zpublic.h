#ifndef ZPUBLIC_H
#define ZPUBLIC_H
#include <QList>
#include <QVariant>

class ZPublic
{
public:
    static QList<QPair<QVariant,QVariant > > getCameras();
    static void test();
};

#endif // ZPUBLIC_H
