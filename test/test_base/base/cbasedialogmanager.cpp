#include "cbasedialogmanager.h"

CBaseDialogManager::~CBaseDialogManager()
{
}

void CBaseDialogManager::closeDialog(const QUuid &uuid)
{
    QSharedPointer<CBaseDialog> dialog = takeDialog(uuid, m_showDialogList);

    if(!dialog.isNull())
    {
        dialog.data()->close();
    }

    if(!dialog.isNull())
    {
        appendDeleteDailog(dialog);
    }
}

CBaseDialogManager::CBaseDialogManager(QObject *parent)
    : QObject(parent)
{
}

void CBaseDialogManager::appendDeleteDailog(QSharedPointer<CBaseDialog> dialog)
{
    //todo 是否加锁
    m_waitDeleteDialogList.append(dialog);
}

QSharedPointer<CBaseDialog> CBaseDialogManager::takeDialog(const QUuid &uuid, QList<QSharedPointer<CBaseDialog> > &dialogList)
{
    QSharedPointer<CBaseDialog> dialog;
    for(int i = 0; i < dialogList.count(); ++i)
    {
        if(dialogList.at(i).data()->getUuid() == uuid)
        {
            dialog = dialogList.takeAt(i);
            break;
        }
    }
    return dialog;
}
