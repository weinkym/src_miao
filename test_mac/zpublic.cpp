#include "zpublic.h"
#include <QTimer>
#include <QEventLoop>

#include <QtAV/QtAV_Global.h>

#include <QApplication>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtAV/QtAV>

#include <QtAVWidgets/QtAVWidgets>


using namespace QtAV;
class Thread : public QThread
{
public:
    Thread(AVPlayer *player):
        QThread(0)
      , mpPlayer(player)
    {}
protected:
    virtual void run() {
        //mpPlayer->play();
        exec();
    }
    AVPlayer *mpPlayer;
};



void ZPublic::testAll()
{
    AVPlayer player;
    WidgetRenderer renderer;
    renderer.show();
    player.addVideoRenderer(&renderer);
    player.setFile("/Users/miaozw/Movies/云学堂.mp4");
    Thread thread(&player);
    player.moveToThread(&thread);
    thread.start();
    player.play();
    QEventLoop loop;
    QTimer::singleShot(10000,&loop,SLOT(quit()));
    loop.exec();
}


//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);

//    AVPlayer player;
//    WidgetRenderer renderer;
//    renderer.show();
//    player.addVideoRenderer(&renderer);
//    player.setFile(a.arguments().last());
//    Thread thread(&player);
//    player.moveToThread(&thread);
//    thread.start();
//    player.play();
////    player.setSpeed(2);

//    return a.exec();
//}
