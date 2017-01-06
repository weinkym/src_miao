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
//    view->installEventFilter(this);
//    editorBar->installEventFilter(this);
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

void MainWindow::on_pushButton_clicked()
{
    view->setBackgroundColor(QColor(233,255,255));
    view->setExpanding(true);
}

void MainWindow::on_horizontalSlider_actionTriggered(int action)
{

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
//    qDebug()<<value;
//    static int v = 100;


//    qreal sx = 0.01 * v;
//    qreal sy = 0.01 * v;;
//    QTransform form = view->transform();
//    form.setMatrix(sx,form.m12(),form.m13(),form.m21(),sy,form.m23(),form.m31(),form.m32(),form.m33());
//    form.scale(sx,sy);
////    form.set
//    view->setTransform(form);
//    v++;
//    if(v > 600)
//    {
//        v = 1;
//    }
}
