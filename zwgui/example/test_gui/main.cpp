#include "mainwindow.h"
#include "testzwlabeldialog.h"
#include "zwframelesswidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //    MainWindow w;
    //    TestZWLabelDialog w;
    ZWFramelessWidget w;
    w.SetResizeable(false);
    w.SetRadius(20);
    w.show();

    return a.exec();
}
