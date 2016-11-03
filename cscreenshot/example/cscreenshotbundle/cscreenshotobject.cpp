#include "cscreenshotobject.h"
#include "cscreenshotmanager.h"
#include <QFile>
#include <QPixmap>
#include <QBuffer>
#include <QTime>

CScreenShotObject::CScreenShotObject(QObject *parent)
    :QObject(parent)
{
    connect(CScreenShotManager::getInstance(),SIGNAL(sigScreenShotPixmapChanged(QPixmap)),
            this,SLOT(onScreenShotPixmapChanged(QPixmap)));
}

CScreenShotObject::~CScreenShotObject()
{
    //
}

void CScreenShotObject::startScreenShot()
{
    QTime time=QTime::currentTime();
    CScreenShotManager::getInstance()->startScreenShot();
    C_SCREENSHOTSHARED_LOG(QString("time=%1").arg(time.msecsTo(QTime::currentTime())));
}

void CScreenShotObject::onScreenShotPixmapChanged(const QPixmap &pixmap)
{
    C_SCREENSHOTSHARED_LOG(QString("pixmap size = %1,%2").arg(pixmap.size().width()).arg(pixmap.height()))
    if(pixmap.isNull())
    {
        return;
    }

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG"); // writes pixmap into bytes in PNG format

    QFile fileout;
    fileout.open(stdout, QIODevice::WriteOnly);
    fileout.write(bytes, bytes.size()); // write to stderr
    fileout.close();
}
