#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(0);
//    trayIcon->showMessage("titleEdit->text()", "bodyEdit->toPlainText()", icon,1 * 1000);

//    NSUserNotification *notification = [[NSUserNotification alloc] init];
//    notification.title = [NSString stringWithUTF8String:title.toUtf8().data()];
//    notification.informativeText = [NSString stringWithUTF8String:message.toUtf8().data()];

//    [[NSUserNotificationCenter defaultUserNotificationCenter] deliverNotification:notification];
    znotification();
}
