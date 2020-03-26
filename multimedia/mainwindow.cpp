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
    QVariantMap m_params;
    m_params["width"] = 1920;
        m_params["height"] = 1080;
        m_params["fps"] = 30;
        m_params["filePath"] = QStringLiteral("/Users/miaozw/Documents/TEMP/test-out.flv");

//        m_recordThread.Init(m_params);

    m_recordThread.Start();
}

void MainWindow::on_pushButton_2_clicked()
{
    m_recordThread.Stop();
}

void MainWindow::on_pushButton_3_clicked()
{
    m_recordThread.Pause();

}
