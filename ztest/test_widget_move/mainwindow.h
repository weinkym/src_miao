#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "zmovedialog.h"
#include <QDebug>
#include <QMainWindow>

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

protected:
    bool event(QEvent *event);
    void moveEvent(QMoveEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    ZMoveDialog *dlg;
};

#endif // MAINWINDOW_H
