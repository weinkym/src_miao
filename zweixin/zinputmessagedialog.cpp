#include "zinputmessagedialog.h"
#include "ui_zinputmessagedialog.h"
#include <QMetaEnum>

ZInputMessageDialog::ZInputMessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZInputMessageDialog)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    QMetaEnum me = QMetaEnum::fromType<Zpublic::AutoSendEventType>();
    ui->comboBox->clear();
    for(int i = 0; i <me.keyCount(); i++)
    {
        ui->comboBox->addItem(me.key(i),me.value(i));
    }
}

ZInputMessageDialog::~ZInputMessageDialog()
{
    delete ui;
}
