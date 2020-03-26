#include <QApplication>
#include "mainwindow.h"
#include "ztextcodec.h"
#include "mplib.h"
#include <QtGlobal>

class A{

};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler (ZMsgHandler);
//    qInstallMsgHandler(ZMsgHandler);
    ZTextCodec codec;
    codec.init("");
    MainWindow w;
    QFile file(":/stylesheet.css");
    if(file.open(QIODevice::ReadOnly))
    {
        qDebug()<<QString("打开文件<%1>成功").arg(file.fileName());
        qApp->setStyleSheet(file.readAll());
    }
    else
    {
        qDebug()<<QString("打开文件<%1>失败").arg(file.fileName());
    }
//    int* a = 0;
//    qDebug()<<"sizeof "<<sizeof(A);
//    return -1;
    w.show();
    return a.exec();
}
