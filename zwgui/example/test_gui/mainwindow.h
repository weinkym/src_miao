#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "zwlog.h"
#include "zwcore.h"
#include "zwgui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    void updatePoints();

private:
    Ui::MainWindow *ui;
    QList<QPointF> m_points;
};

#endif // MAINWINDOW_H
