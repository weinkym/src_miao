#include "zinputmessagedialog.h"
#include "ui_zinputmessagedialog.h"
#include "cmessageinterface.h"

#include <QMetaEnum>

const char *g_field_string = "----";
ZInputMessageDialog::ZInputMessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZInputMessageDialog)
{
    ui->setupUi(this);
//    ui->textEditAutoContent->setReadOnly(true);
    ui->textEditAutoContent->clear();

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

void ZInputMessageDialog::resetContacts(const QList<QSharedPointer<Z_WX_USER_DATA> > &contactList)
{
    ui->listWidgetContact->resetContacts(contactList);
}

QString ZInputMessageDialog::getCurrentUserName()
{
    QSharedPointer<Z_WX_USER_DATA> pObj = ui->listWidgetContact->getCurrentContact();
    if(pObj.isNull())
    {
        return "";
    }
    return pObj.data()->UserName;
}

void ZInputMessageDialog::on_btnAdd_clicked()
{
    QStringList objList = ui->textEditAutoContent->toPlainText().split("\n");
    qDebug()<<"SDFASDFADSF"<<objList.count();
//    qDebug()<< QDate::currentDate();
//    QDate date(1970,1,1);
//     date = date.addDays(123213);
//     qDebug()<< date;

    QString userName = getCurrentUserName();
    foreach(QString obj,objList)
    {
        QStringList fieldList = obj.split(g_field_string,QString::SkipEmptyParts);
        if(fieldList.count() == 3)
        {
            CPB::AutoSendEventData  msg;
            msg.toUserName = userName;
            msg.content=fieldList.at(0);
            int days = QString(fieldList.at(1)).toInt();
            msg.type=QString(fieldList.at(2)).toInt();
            QDate date(1900,1,1);
             date = date.addDays(days-2);

            msg.dateTime=QDateTime(date,QTime()).toTime_t();
//            QDate date;
//            date.addDays(days);

             qDebug()<< QDateTime(date,QTime());
             msg.uuid = QUuid::createUuid().toString();
             CMessageInterface::getInstance()->addMessage(msg);
//            qDebug()<<"d.dateTime"<< (QDateTime(date,QTime()).toString("yyyyMMdd hh:mm:ss"));
        }
    }
//    ui->textEditAutoContent->clear();

//    QString userName = getCurrentUserName();
//    ui->textEditAutoContent->append(QString("%1----%2----%3----%4")
//                                    .arg(ui->dateTimeEdit->dateTime().toString("yyyyMMdd hh:mm:ss"))
//                                    .arg(ui->comboBox->currentData().toInt())
//                                    .arg(ui->textEditContent->toPlainText())
//                                    .arg(userName));
}
