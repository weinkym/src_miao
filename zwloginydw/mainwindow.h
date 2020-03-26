#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QTimer>
#include "zwwebengineview.h"
#include "zwjsbridgeobject.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pbLoadUrl_clicked();

    void on_pbRunJS_clicked();

    void testToJS();
    void testToJS2(const QString &test,int number);
    int testToJS3(const QString &test,int number);
    QString testToJS4(const QString &test,int number);

private slots:
    void on_pushButtonAuto_clicked();
    void onTimeout();

private:
    Ui::MainWindow *ui;
    ZWWebengineView *m_view;
    ZWJSBridgeObject *m_bridgeObject;
    QTimer *m_runTimer;
//    QWebEngineView *m_textView;
};

#endif // MAINWINDOW_H
