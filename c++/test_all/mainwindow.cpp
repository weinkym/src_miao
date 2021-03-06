#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ZPlatformMac::doTest();
    QTimer::singleShot(10000,this,[]
    {
        auto screenList = QApplication::screens();
        for(auto obj : screenList)
        {
            ZW_LOG_INFO(ZW_LOG_P(obj->geometry()));
        }
        QWidget *w = new QWidget;
        QPushButton *btn = new QPushButton("close",w);
        w->setMinimumSize(100,100);
        w->move(100,3);
        btn->move(30,30);
        connect(btn,SIGNAL(clicked(bool)),w,SLOT(close()));
    //    w->setWindowFlags(Qt::FramelessWindowHint);
    //    w->setWindowFlags(w->windowFlags() | Qt::CustomizeWindowHint);
        w->setWindowFlags(Qt::CustomizeWindowHint);
        w->setAttribute(Qt::WA_DeleteOnClose);
//        ZPlatformMac::setWidgetLevel(w);
        ZPlatformMac::setWidgetLevel(w);
        w->show();
    });

}
