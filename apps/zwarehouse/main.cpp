#include "mainwindow.h"
#include <QApplication>
#include "zdbmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZDBManager::getInstance()->init();
    MainWindow w;
    w.show();

    return a.exec();
}
