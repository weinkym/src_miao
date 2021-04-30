#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QTextEdit>
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QPixmap>
#include <QScreen>
#include <QPushButton>

#if defined(Q_OS_MAC)
#include "zplatformmac.h"
#endif
#include "ztestviwerall.h"
#include "zwlog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
