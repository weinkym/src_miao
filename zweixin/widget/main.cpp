#include "mainwindow.h"
#include <QApplication>
#include <QTextEdit>
#include "csqliteaccessinterface.h"
#include "cmessageinterface.h"
#include "zinputmessagedialog.h"
#include <QMetaEnum>
#include "ccontactmanager.h"
#include "zcontactlistwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime::currentTime().msec());
    MainWindow w;
    w.hide();
    w.showLogin();
    return a.exec();
}
