#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zwvideothread.h"
#include "zwlog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);
    m_videoThread = new ZWVideoThread();
    connect(m_videoThread,SIGNAL(sigImageChanged(QImage)),this,SLOT(onImageChanged(QImage)));
    connect(m_videoThread,SIGNAL(sig_GetOneFrame(QImage)),this,SLOT(onImageChanged(QImage)));
    connect(m_videoThread,SIGNAL(sigErrorHappened(int,QString)),this,SLOT(onErrorHappened(int,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onErrorHappened(int type, const QString &errorString)
{
    ZW_LOG_WARNING(QString("type=%1,error=%2").arg(type).arg(errorString));
}

void MainWindow::onImageChanged(const QImage &image)
{
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_pushButton_clicked()
{
    m_videoThread->pause();
}

void MainWindow::on_pushButton_2_clicked()
{
    m_videoThread->play();
    if(!m_videoThread->isRunning())
    {
        m_videoThread->start();
    }
}
