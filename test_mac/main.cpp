#include "mainwindow.h"
#include "zwlog.h"
#include <QApplication>
#include <typeinfo>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
