#include "mainwindow.h"
#include <QApplication>
#include "zwwebenginepage.h"
#include "zwwebengineview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    ZWWebengineView w;
//    w.load(QUrl("https://www.baidu.com"));
    w.show();
    return a.exec();
}
