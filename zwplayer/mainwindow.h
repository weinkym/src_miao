#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ZWVideoThread;

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
    void onErrorHappened(int type,const QString &errorString);
    void onImageChanged(const QImage &image);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    ZWVideoThread *m_videoThread;
};

#endif // MAINWINDOW_H
