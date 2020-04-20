#include "cdialogmanager.h"

CDialogManager *CDialogManager::m_instance = NULL;

CDialogManager::~CDialogManager()
{
}

CDialogManager *CDialogManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CDialogManager;
    }
    return m_instance;
}

void CDialogManager::closeDialog(const QUuid &uuid)
{
    QSharedPointer<CDialog> dialog = takeDialog(uuid, m_showDialogList);

    if(!dialog.isNull())
    {
        dialog.data()->close();
    }

    if(!dialog.isNull())
    {
        appendDeleteDailog(dialog);
    }
}

CDialogManager::CDialogManager(QObject *parent)
    : QObject(parent)
{
}

void CDialogManager::appendDeleteDailog(QSharedPointer<CDialog> dialog)
{
    //todo 是否加锁
    m_waitDeleteDialogList.append(dialog);
}

QSharedPointer<CDialog> CDialogManager::takeDialog(const QUuid &uuid, QList<QSharedPointer<CDialog> > &dialogList)
{
    QSharedPointer<CDialog> dialog;
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
