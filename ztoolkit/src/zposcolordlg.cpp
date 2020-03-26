#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "zposcolordlg.h"
#include "ui_zposcolordlg.h"
#include "mplib.h"

ZPosColorDlg::ZPosColorDlg(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent,f)
{
    ui = new Ui_ZPosColorDlg;
    ui->setupUi(this);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(updateColor()));
    connect(ui->btn,SIGNAL(clicked()),this,SLOT(onBtnClicked()));
    m_timer.setInterval(50);
}

ZPosColorDlg::~ZPosColorDlg()
{
    //
}

void ZPosColorDlg::updateColor()
{
    QColor color = Mplib::MpStaticMethod::getCursorPosColor();
    QString text = QString("R:%1 G:%2 B:%3 name:%4 alpha:%5")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue())
            .arg(color.name())
            .arg(color.alpha());

    ui->label->setText(text);
    ui->lineEdit->setText(text);
}

void ZPosColorDlg::onBtnClicked()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        //开始
        ui->btn->setText("start");
    }
    else
    {
        m_timer.start();
        //开始
        ui->btn->setText("pause");
    }
}
