#ifndef CBASEDIALOGMANAGER_H
#define CBASEDIALOGMANAGER_H
#include "cbasedialog.h"
#include <QObject>

class CBaseDialogManager : public QObject
{
    Q_OBJECT
public:
    ~CBaseDialogManager();
    void closeDialog(const QUuid &uuid);

protected:
    explicit CBaseDialogManager(QObject *parent = Q_NULLPTR);
    void appendDeleteDailog(QSharedPointer<CBaseDialog> dialog);
    QUuid showDialog(QSharedPointer<CBaseDialog> dialog);

private:
    QSharedPointer<CBaseDialog> takeDialog(const QUuid &uuid, QList<QSharedPointer<CBaseDialog> > &dialogList);

private slots:
    void onDialogClosed(const QUuid &uuid, int showType);

private:
    QList<QSharedPointer<CBaseDialog> > m_showDialogList;
    QList<QSharedPointer<CBaseDialog> > m_waitDeleteDialogList;
};

#endif // CBASEDIALOGMANAGER_H
