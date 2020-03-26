#include "zcontactlistwidget.h"
#include "ui_zcontactlistwidget.h"
#include "zcontackitemwidget.h"

ZContactListWidget::ZContactListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZContactListWidget)
{
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //TODO
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(on_btnSearh_clicked()));
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
    ui->listWidget->setCurrentRow(0);
}

QSharedPointer<Z_WX_USER_DATA> ZContactListWidget::getCurrentContact()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    ZContackItemWidget *itemWidget = (ZContackItemWidget*)ui->listWidget->itemWidget(item);
    if(itemWidget)
    {
        return itemWidget->getContact();
    }
    return QSharedPointer<Z_WX_USER_DATA>(NULL);
}

void ZContactListWidget::on_btnSearh_clicked()
{
    QString key = ui->lineEdit->text();
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        if(key.isEmpty())
        {
            ui->listWidget->item(i)->setHidden(false);
        }
        else
        {
            QListWidgetItem *item = ui->listWidget->item(i);
            ZContackItemWidget *itemWidget = (ZContackItemWidget*)ui->listWidget->itemWidget(item);
            item->setHidden(!(itemWidget && itemWidget->isContain(key)));
        }
    }
}
