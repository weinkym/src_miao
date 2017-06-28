#ifndef CUTILINTERFACE_H
#define CUTILINTERFACE_H

#include "cpublicdefined.h"

class CUtilInterface
{
public:
    CUtilInterface();
    QList<CYDW::KEY_DATA> getTableInfo();
};

#endif // CUTILINTERFACE_H
