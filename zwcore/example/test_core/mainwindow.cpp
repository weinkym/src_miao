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
    ZW_LOG::initLog("/Users/miaozw/Documents/TEMP/test.log");
    ZW_LOG_DEBUG("asdfasdfadsf");
    ZW_LOG_FUNCTION;
}
