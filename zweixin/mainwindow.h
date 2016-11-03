#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "zgolbal.h"
#include "zpublic.h"
#include "zpublicaction.h"
#include "zpeerobjectdata.h"

class ZLoginWidget;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showLogin();

private:
    void requestInit();
    bool parseInitData(const QByteArray &byteArray);

private slots:
    void onFinished(const ZRequestAction::ZRequestResponse &response);
    void on_btnTest01_clicked();
    void onLoginFinished(bool ok);
    void onTest();

private:
    Ui::MainWindow *ui;
    ZLoginWidget *m_loginWidget;
    ZBaseRequestParam m_baseRequestParam;
    ZPeerObjectData m_userAvatar;
    Z_WX_USER_DATA m_userData;
};

#endif // MAINWINDOW_H
