#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_view = new ZWWebengineView;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_view);
    ui->tabWidget->currentWidget()->setLayout(hLayout);
//    m_view->load(QUrl("http://www.yidai.com/user/login/"));
    m_view->startLogin();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    m_view->page()->runJavaScript("alert(\"你好，我是一个警告框！\")");
    QFile file("/Users/miaozw/Documents/TEMP/test.js");

    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray data = file.readAll();
    file.close();
//    qDebug()<<"data";
//    qDebug()<<data;
    m_view->page()->runJavaScript(QString(data));

}

void MainWindow::on_pushButton_2_clicked()
{
    m_view->page()->load(QUrl("https://www.yidai.com/borrowesharemy/index/"));
//    m_view->startLogin();
}
