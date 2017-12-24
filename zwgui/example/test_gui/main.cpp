#include "mainwindow.h"
#include <QApplication>
#include "testzwlabeldialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    TestZWLabelDialog w;
    w.show();

    return a.exec();
}
