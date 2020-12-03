#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zpublic.h"
#include <QAudioInput>
#include <QCamera>
#include <QCameraInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_timer, SIGNAL(timeout()), SLOT(onTest()));
    m_timer.setInterval(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //    if(m_timer.isActive())
    //    {
    //        m_timer.stop();
    //    }
    //    else
    //    {
    //        m_timer.start();
    //    }
    //     QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    //     foreach (const QCameraInfo &cameraInfo, cameras) {
    //         qDebug()<<cameraInfo.deviceName()<<cameraInfo.description()<<cameraInfo.position();
    //     }

    //     QList<QAudioDeviceInfo> infos = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    //     foreach (const QAudioDeviceInfo &info, infos) {
    //         qDebug()<<info.deviceName();
    //     }
    //    qDebug()<<QSysInfo::prettyProductName()<<QSysInfo::macVersion();
    //    ZPublic::testAll();
    //    ZPublic::getCameras();
    //    ZPublic::test();
    ZPublic::testAll();
}

void MainWindow::onTest()
{
    //    ZPublic::getCameras();
}
