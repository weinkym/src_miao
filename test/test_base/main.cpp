
#include "clogsetting.h"
#include "ctestwidget.h"
#include "mainwindow.h"
#include <QAbstractItemModel>
#include <QApplication>

void test()
{
    QSize size(6000, 4000);
    QSize toSize(1280, 720);
    size = size.scaled(toSize, Qt::IgnoreAspectRatio);
    //    size = size.scaled(toSize, Qt::KeepAspectRatio);
    //    size = size.scaled(toSize, Qt::KeepAspectRatio);
    qDebug() << size;
}

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    //    test();
    MainWindow w;
    w.show();
    return app.exec();
}
