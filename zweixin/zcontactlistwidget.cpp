#include "zcontactlistwidget.h"
#include "ui_zcontactlistwidget.h"
#include "zcontackitemwidget.h"

ZContactListWidget::ZContactListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZContactListWidget)
{
    ui->setupUi(this);
}

ZContactListWidget::~ZContactListWidget()
{
    delete ui;
}

void ZContactListWidget::resetContacts(const QList<QSharedPointer<Z_WX_USER_DATA> > &contactList)
{
    ui->listWidget->clear();

    QSize size(200,50);

    for(auto obj:contactList)
    {
        QListWidgetItem *item = new QListWidgetItem;
        ZContackItemWidget *itemWidget = new ZContackItemWidget(obj);
        itemWidget->setMinimumSize(size);
        itemWidget->setMaximumSize(size);
        item->setSizeHint(itemWidget->size());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item,itemWidget);
    }
}
