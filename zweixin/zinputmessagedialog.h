#ifndef ZINPUTMESSAGEDIALOG_H
#define ZINPUTMESSAGEDIALOG_H

#include <QDialog>
#include "zpublic.h"

namespace Ui {
class ZInputMessageDialog;
}

class ZInputMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZInputMessageDialog(QWidget *parent = 0);
    ~ZInputMessageDialog();
    void resetContacts(const QList<QSharedPointer<Z_WX_USER_DATA> > &contactList);

private:
    QString getCurrentUserName();

private slots:
    void on_btnAdd_clicked();

private:
    Ui::ZInputMessageDialog *ui;
};

#endif // ZINPUTMESSAGEDIALOG_H
