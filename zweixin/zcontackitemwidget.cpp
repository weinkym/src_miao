#include "zcontackitemwidget.h"
#include "ui_zcontackitemwidget.h"

ZContackItemWidget::ZContackItemWidget(const QSharedPointer<Z_WX_USER_DATA> contact, QWidget *parent)
    : QWidget(parent)
     ,ui(new Ui::ZContackItemWidget)
    ,m_contact(contact)
{
    ui->setupUi(this);
    if(!m_contact.isNull())
    {
        QString name = m_contact.data()->RemarkName;
        if(name.isEmpty())
        {
            name = m_contact.data()->NickName;
        }
        ui->label->setText(name);
        ui->labelName->setText(name);
        ui->labelName2->setText(m_contact.data()->Signature);
        ui->labelName->setVisible(false);
        ui->labelName2->setVisible(false);
    }
}

ZContackItemWidget::~ZContackItemWidget()
{
    delete ui;
}

QSharedPointer<Z_WX_USER_DATA> ZContackItemWidget::getContact()
{
    return m_contact;
}
