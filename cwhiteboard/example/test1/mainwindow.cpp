#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QWidget* view = NULL;
//    QWidget* editorBar = NULL;
    createWhiteBoardWidgets(&view,&editorBar);
    ui->vLayout->insertWidget(0,editorBar);
    ui->vLayout->insertWidget(0,view);
    ui->vLayout->setSpacing(0);
    view->installEventFilter(this);
    editorBar->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        if(obj == view)
        {
            qDebug()<<"TTTTTTTTTTTT";
        }
        if(obj == editorBar)
        {
            qDebug()<<"aaaaaaaaa";
//            e->accept();
//            return true;
        }
    }
    return QMainWindow::eventFilter(obj,e);
}
