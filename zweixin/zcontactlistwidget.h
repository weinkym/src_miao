#ifndef ZCONTACTLISTWIDGET_H
#define ZCONTACTLISTWIDGET_H

#include <QWidget>
#include "zpublic.h"

namespace Ui {
class ZContactListWidget;
}

class ZContactListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZContactListWidget(QWidget *parent = 0);
    ~ZContactListWidget();
    void resetContacts(const QList<QSharedPointer<Z_WX_USER_DATA> > &contactList);

private:
    Ui::ZContactListWidget *ui;
};

#endif // ZCONTACTLISTWIDGET_H
