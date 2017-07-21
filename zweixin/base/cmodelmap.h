#ifndef CMODELMAP_H
#define CMODELMAP_H

#include <QHash>
#include "csqliteaccessinterface.h"

class CModelMap
{
public:

protected:
    CModelMap():m_db(nullptr){}
    ~CModelMap(){}

private:
    CSqliteAccessInterface *m_db;                  /*!< 数据库访问指针 */
};

#endif // CMODELMAP_H
