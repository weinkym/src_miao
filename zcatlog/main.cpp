#include "mainwindow.h"
#include "zcatlogwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    ZCatlogWidget w;
    w.show();
    
    return a.exec();
}
