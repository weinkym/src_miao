#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

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
{/*
    int *a;
    *a = 100;*/
//    QWidget *w;
//    w->setWindowTitle("asdfasdf");
//    w->show();
    int b = 0;
    int a = 10 / b;
    qDebug()<<a;
}
