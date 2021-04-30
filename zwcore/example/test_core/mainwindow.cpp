#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ZW_LOG::initLog("/Users/miao/Documents/TEMP/test.log");
    ZW_LOG_DEBUG("asdfasdfadsf");
    ZW_LOG_FUNCTION;
    ZW_LOG_INFO(ZWCore::convertToPinyin("缪正伟DFADSF"));
    QColor color(123,11,33);
    ZW_LOG_INFO(ZW_LOG_P(color));
}
