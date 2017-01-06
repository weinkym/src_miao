#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cwhiteboard.h"

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
    bool eventFilter(QObject *obj, QEvent *e);

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_actionTriggered(int action);

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    CWhiteBoardView* view;
    CWhiteBoardEditorBar* editorBar;
};

#endif // MAINWINDOW_H
