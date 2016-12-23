#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* view = NULL;
    QWidget* editorBar = NULL;
    createWhiteBoardWidgets(&view,&editorBar);
    ui->vLayout->insertWidget(0,editorBar);
    ui->vLayout->insertWidget(0,view);
    ui->vLayout->setSpacing(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
