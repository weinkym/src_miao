#include <QApplication>
#include "cscreenshotmanager.h"
#include "cscreenshotobject.h"

#include <QUuid>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CScreenShotObject obj;

    QStringList arguments = a.arguments();
    qDebug()<<arguments.join("-------");
    bool needCheck = false;
    if(needCheck)
    {

        if(arguments.count() < 2)
        {
            qDebug()<<QString("input start key");
            return 1;
        }

        QString startKey = "c8a55807-c95a-490e-91c0-74bdbc129904";
        if(arguments.at(1) != startKey)
        {
            qDebug()<<QString("start key is error");
            return 2;
        }
        if(obj.isStart(startKey))
        {
            return 3;
        }
    }
    obj.startScreenShot();

    return a.exec();
}
