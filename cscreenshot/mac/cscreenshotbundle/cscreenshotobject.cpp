#include "cscreenshotobject.h"
#include "cscreenshotmanager.h"
#include <QFile>
#include <QPixmap>
#include <QBuffer>
#include <QTime>
#include <QLocalServer>
#include <QLocalSocket>

CScreenShotObject::CScreenShotObject(QObject *parent)
    :QObject(parent)
    ,m_localServer(NULL)
{
    connect(CScreenShotManager::getInstance(),SIGNAL(sigScreenShotPixmapChanged(QPixmap,QRect,QRect)),
            this,SLOT(onScreenShotPixmapChanged(QPixmap)));
}

CScreenShotObject::~CScreenShotObject()
{
    //
}

void CScreenShotObject::startScreenShot()
{
    C_SCREENSHOT_LOG_TEST;
    QTime time=QTime::currentTime();
    CScreenShotManager::getInstance()->startScreenShot();
    C_SCREENSHOT_LOG_INFO(QString("time=%1").arg(time.msecsTo(QTime::currentTime())));
}

bool CScreenShotObject::isStart(const QString &account)
{
    C_SCREENSHOT_LOG_TEST;
    QLocalSocket socket;
    QString serverName = account;

    socket.connectToServer(serverName);

    if(socket.waitForConnected())
    {
        return true;
    }
    else
    {
        if(!m_localServer)
        {
            m_localServer = new QLocalServer(this);
            if(!m_localServer->listen(serverName))
            {
                // 此时监听失败，可能是程序崩溃时,残留进程服务导致的,移除之
                QAbstractSocket::SocketError socketError = m_localServer->serverError();
                if(socketError == QAbstractSocket::AddressInUseError)
                {
                    QLocalServer::removeServer(serverName);
                    m_localServer->listen(serverName); // 再次监听
                }
            }
        }
    }

    return false;
}

void CScreenShotObject::onScreenShotPixmapChanged(const QPixmap &pixmap)
{
    C_SCREENSHOT_LOG_INFO(QString("pixmap size = %1,%2").arg(pixmap.size().width()).arg(pixmap.height()));
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
