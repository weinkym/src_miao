#include "widget.h"
#include <QApplication>
#include "cscreenshotmanager.h"
#include "cscreenshotobject.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
    CScreenShotObject obj;
    obj.startScreenShot();
    return a.exec();
}
