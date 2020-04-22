#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cbaseutils.h"
#include "ctestdialog.h"
#include "cuiutils.h"
#include <QDataStream>
#include <QDateTime>
#include <QMainWindow>
#include <QTime>
#include <QTimer>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void onTestSlot();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
