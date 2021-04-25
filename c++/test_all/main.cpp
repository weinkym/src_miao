#include "mainwindow.h"

#include <QApplication>
#include "ztestall.h"
#include "ztestviwerall.h"
#include "zproduceconsumer.h"

int main(int argc, char *argv[])
{
//    ZTestAll::testAll();
//    ZProduceConsumer::test();
//    return 0;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
