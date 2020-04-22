#ifndef CDIALOGMANAGER_H
#define CDIALOGMANAGER_H

#include "cbasedialogmanager.h"
#include <QObject>

class CDialogManager : public CBaseDialogManager
{
    Q_OBJECT
public:
    CDialogManager *getInstance();
    ~CDialogManager();
    void closeDialog(const QUuid &uuid);

private:
    explicit CDialogManager(QObject *parent = Q_NULLPTR);

private:
    static CDialogManager *m_instance;
};

#endif // CDIALOGMANAGER_H
