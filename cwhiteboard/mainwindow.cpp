#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cwhiteboardview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    m_view = new CWhiteBoardView;
    //    ui->vLayout->insertWidget(0,m_view);
    QWidget *view = NULL;
    QWidget *eidtorBar = NULL;
    createWhiteBoardWidgets(&view,&eidtorBar);
//    eidtorBar->setMinimumHeight(90);
    ui->vLayout->insertWidget(0,eidtorBar);
    ui->vLayout->insertWidget(0,view);
    ui->vLayout->setSpacing(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}
