#include "cbasedialogmanager.h"
#include "clogsetting.h"

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
    if(dialog.isNull())
    {
        C_LOG_WARNING("dialog is null");
        return;
    }
    //todo 是否加锁
    m_waitDeleteDialogList.append(dialog);
}

QUuid CBaseDialogManager::showDialog(QSharedPointer<CBaseDialog> dialog)
{
    if(dialog.isNull())
    {
        C_LOG_WARNING("dialog is null");
        return QUuid();
    }
    if(dialog.data()->getShowType() == CBaseDialog::SHOW_TYPE_NORMAL_MODAL)
    {
        C_LOG_WARNING("dialog show type normal modal,it will be ignore");
        return QUuid();
    }
    connect(dialog.data(), SIGNAL(sigClosed(QUuid, int)), this, SLOT(onDialogClosed(QUuid, int)));
    m_showDialogList.append(dialog);
    dialog.data()->startShow();
    return dialog.data()->getUuid();
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

void CBaseDialogManager::onDialogClosed(const QUuid &uuid, int showType)
{
    C_LOG_OUT_V2(uuid, showType);
    QSharedPointer<CBaseDialog> dialog = takeDialog(uuid, m_showDialogList);
    appendDeleteDailog(dialog);
}
