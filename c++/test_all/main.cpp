#include "mainwindow.h"

#include <QApplication>
#include "ztestall.h"

int main(int argc, char *argv[])
{
    ZTestAll::testSort();
    return 0;
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
}
