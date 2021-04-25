#include "mainwindow.h"
#include "testzwlabeldialog.h"
#include "zwframelesswidget.h"
#include <QApplication>
#include "zwmovewidgethandler.h"
#include "zwmovewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //    MainWindow w;
    //    TestZWLabelDialog w;
//    ZWFramelessWidget w;
//    w.SetResizeable(false);
    ZWMoveWidget w;
//    w.SetResizeable(true);
//    w.SetRadius(5);
    w.show();

    return a.exec();
}
