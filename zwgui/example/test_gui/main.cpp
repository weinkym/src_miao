#include "mainwindow.h"
#include <QApplication>
#include "testzwlabeldialog.h"
#include "zwframelesswidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    TestZWLabelDialog w;
    ZWFramelessWidget w;
    w.show();

    return a.exec();
}
