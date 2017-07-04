#ifndef ZCONTACKITEMWIDGET_H
#define ZCONTACKITEMWIDGET_H

#include <QWidget>
#include "zpublic.h"

namespace Ui {
class ZContackItemWidget;
}

class ZContackItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZContackItemWidget(const QSharedPointer<Z_WX_USER_DATA> contact,QWidget *parent = 0);
    ~ZContackItemWidget();
    QSharedPointer<Z_WX_USER_DATA> getContact();

private:
    Ui::ZContackItemWidget *ui;
    QSharedPointer<Z_WX_USER_DATA> m_contact;
};

#endif // ZCONTACKITEMWIDGET_H
