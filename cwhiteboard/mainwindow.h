#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CWhiteBoardView;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CWhiteBoardView *m_view;
};

#endif // MAINWINDOW_H
