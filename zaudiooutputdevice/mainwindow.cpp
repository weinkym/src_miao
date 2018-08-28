#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
//  ,m_audioOutput(NULL)
//  ,m_PCMIODevice(NULL)
{
    ui->setupUi(this);

    const QAudioDeviceInfo &defaultDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();
    ui->comboBox->addItem(defaultDeviceInfo.deviceName(), qVariantFromValue(defaultDeviceInfo));
    foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)) {
        if (deviceInfo != defaultDeviceInfo)
            ui->comboBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbTest_clicked()
{
    QAudioDeviceInfo device = ui->comboBox->currentData().value<QAudioDeviceInfo>();
    m_test.start(ui->lineEditPath->text(),device);
//    m_device = QAudioDeviceInfo::defaultOutputDevice();
//    if(m_audioOutput)
//    {
//        delete m_audioOutput;
//        m_audioOutput = 0;
//    }
//    if(m_PCMIODevice)
//    {
//        m_PCMIODevice->stop();
//        delete m_PCMIODevice;
//        m_PCMIODevice = 0;
//    }

//    m_format.setSampleRate(44100);
//    m_format.setChannelCount(1);
//    m_format.setSampleSize(16);
//    m_format.setCodec("audio/pcm");
//    m_format.setByteOrder(QAudioFormat::LittleEndian);
//    m_format.setSampleType(QAudioFormat::SignedInt);

//    QAudioDeviceInfo info(m_device);
//    if (!info.isFormatSupported(m_format)) {
////        qWarning() << "Default format not supported - trying to use nearest";
//        m_format = info.nearestFormat(m_format);
//    }

//    m_PCMIODevice = new CPCMIODevice("/Users/miaozw/Movies/a.pcm", this);
//    m_audioOutput = new QAudioOutput(m_device, m_format, this);
//    m_PCMIODevice->start();
//    m_audioOutput->start(m_PCMIODevice);
//    m_volumeSlider->setValue(int(m_audioOutput->volume()*100.0f));
}

void MainWindow::on_pbLoad_clicked()
{
    ui->lineEditPath->setReadOnly(true);
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Open PCM"), "./", tr("PCM Files (*.pcm)"));
    if(fileName.isEmpty())
    {
        return;
    }
    ui->lineEditPath->setText(fileName);
}
