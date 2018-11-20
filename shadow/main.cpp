#include "mainwindow.h"
#include <QApplication>
#include "zshadowwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    ZShadowWidget w;
    w.setAttribute(Qt::WA_TranslucentBackground);
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
