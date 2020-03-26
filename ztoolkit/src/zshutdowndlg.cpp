#include <QProcess>
#include "zshutdowndlg.h"


ZShutdownDlg::ZShutdownDlg(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
   ui.setupUi(this);
   ui.dateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(3600));
   ui.dateTimeEdit->setReadOnly(true);
   ui.dSpinBox->setValue(1);
   m_timer.setInterval(500);
   connect(&m_timer,SIGNAL(timeout()),this,SLOT(onTimerout()));
   connect(ui.dSpinBox,SIGNAL(valueChanged(double)),this,SLOT(onValueChanged(double)));
}

ZShutdownDlg::~ZShutdownDlg()
{
    //
}

void ZShutdownDlg::on_btn_start_clicked()
{
    if(m_timer.isActive())
    {
        //开始
        ui.btn_start->setText(tr("start"));
        ui.label_time->setVisible(false);
        ui.dSpinBox->setReadOnly(false);
        m_timer.stop();
    }
    else
    {
        //取消
        ui.btn_start->setText(tr("cancel"));
        m_timer.start();
        ui.label_time->setVisible(true);
        ui.dSpinBox->setReadOnly(true);
    }
}

void ZShutdownDlg::onTimerout()
{
    qint64 secs = QDateTime::currentDateTime().secsTo(ui.dateTimeEdit->dateTime());
    if(secs <= 1)
    {
//        QString program = "C:/WINDOWS/system32/shutdown.exe";
        QString program = "shutdown";
        QStringList arguments;
        arguments << "-s";
        QProcess *myProcess = new QProcess();
        myProcess->start(program, arguments);
        return;
    }
    int h = secs / 3600;
    secs %= 3600;
    int m = secs / 60;
    secs %= 60;
    //%1小时%2分%3秒
    ui.label_time->setText(QString("%1hour%2minute%3second").arg(h).arg(m).arg(secs));
}

void ZShutdownDlg::onValueChanged(double value)
{
    qint64 secs = (qint64)(value * 3600);
    ui.dateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(secs));
}
