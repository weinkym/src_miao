#include "mainwindow.h"

#include <QApplication>
#include "ztestall.h"
#include "ztestviwerall.h"
#include "zproduceconsumer.h"

int main(int argc, char *argv[])
{
//    ZTestAll::testAll();
//    ZProduceConsumer::test();
//    return 0;
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    ZPlatformMac::doTest();
    {
        QTimer::singleShot(5000,[]
        {
            auto screenList = QApplication::screens();
            for(auto obj : screenList)
            {
                ZW_LOG_INFO(ZW_LOG_P(obj->geometry()));
            }
            QWidget *w = new QWidget;
            QPushButton *btn = new QPushButton("close",w);
            w->setMinimumSize(100,100);
            w->move(100,3);
            btn->move(30,30);
            QObject::connect(btn,SIGNAL(clicked(bool)),w,SLOT(close()));
//            w->setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint);
        //    w->setWindowFlags(w->windowFlags() | Qt::CustomizeWindowHint);
            w->setWindowFlags(Qt::CustomizeWindowHint);
            w->setAttribute(Qt::WA_DeleteOnClose);
    //        ZPlatformMac::setWidgetLevel(w);
            ZPlatformMac::setWidgetLevel(w);
//            w->setGeometry(screenList.first()->geometry());
//            w->showNormal();
            w->showFullScreen();
        });
    }
    return a.exec();
}
