#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioOutput>
#include "cpcmiodevice.h"
#include "caudiooutputtest.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbTest_clicked();

    void on_pbLoad_clicked();

    void onValueChanged(int value);

private:
    Ui::MainWindow *ui;

//    QAudioDeviceInfo m_device;
//    QAudioOutput *m_audioOutput;
//    QIODevice *m_output; // not owned
//    QAudioFormat m_format;
//    CPCMIODevice *m_PCMIODevice;
    CAudioOutputTest m_test;
};

#endif // MAINWINDOW_H
