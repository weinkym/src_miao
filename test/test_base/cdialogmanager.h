#ifndef CDIALOGMANAGER_H
#define CDIALOGMANAGER_H

#include "cdialog.h"
#include <QObject>

class CDialogManager : public QObject
{
    Q_OBJECT
public:
    CDialogManager *getInstance();
    ~CDialogManager();
    void closeDialog(const QUuid &uuid);

private:
    explicit CDialogManager(QObject *parent = Q_NULLPTR);
    void appendDeleteDailog(QSharedPointer<CDialog> dialog);
    QSharedPointer<CDialog> takeDialog(const QUuid &uuid, QList<QSharedPointer<CDialog> > &dialogList);

private:
    static CDialogManager *m_instance;
    QList<QSharedPointer<CDialog> > m_showDialogList;
    QList<QSharedPointer<CDialog> > m_waitDeleteDialogList;
};

#endif // CDIALOGMANAGER_H
