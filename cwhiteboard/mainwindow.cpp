#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cwhiteboardview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_view = new CWhiteBoardView;
    ui->vLayout->insertWidget(0,m_view);
}

MainWindow::~MainWindow()
{
    delete ui;
}
