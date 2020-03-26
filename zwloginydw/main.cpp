#include "mainwindow.h"
#include <QApplication>
#include "zwwebenginepage.h"
#include "zwwebengineview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList argList = a.arguments();

    qDebug()<<argList.join("=====");
//    if(argList.count() == 4)
//    {
//        ZWWebengineView view;
//        view.startLogin(argList.at(1),argList.at(2),argList.at(3));
//        return a.exec();
//    }
    MainWindow w;
//    ZWWebengineView w;
//    w.load(QUrl("https://www.baidu.com"));
    w.show();
    return a.exec();
}
