#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QMoveEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dlg = new ZMoveDialog();
    dlg->setMaximumSize(QSize(300, 100));
    dlg->setMoveWidget(ui->textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::event(QEvent *event)
{
    //    qDebug() << event->type() << this->geometry().bottomRight();
    if(event->type() == QEvent::NonClientAreaMouseMove)
    {
        //        dlg->move(QCursor::pos() + QPoint(-100, 100));
    }
    return QMainWindow::event(event);
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    //    dlg->move(event->pos() + QPoint(-100, 100));
    //    qDebug() << event->oldPos() << event->pos() << event->oldPos() - event->pos();
    //    return QMainWindow::moveEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << event->pos();
}

void MainWindow::on_pushButton_clicked()
{
    dlg->show();
}
