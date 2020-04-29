#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cbaseutils.h"
#include "cdownloadobject.h"
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
    void onProcessUpdated(quint64 complete, quint64 fileSize);

private:
    Ui::MainWindow *ui;
    CDownloadObject *m_downloadObject;
};

#endif // MAINWINDOW_H
