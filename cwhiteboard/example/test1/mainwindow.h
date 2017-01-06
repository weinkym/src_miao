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

private:
    Ui::MainWindow *ui;
    CWhiteBoardView* view;
    CWhiteBoardEditorBar* editorBar;
};

#endif // MAINWINDOW_H
