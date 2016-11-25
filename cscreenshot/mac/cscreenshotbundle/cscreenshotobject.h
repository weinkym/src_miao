#ifndef CSCREENSHOTOBJECT_H
#define CSCREENSHOTOBJECT_H

#include <QObject>

class QLocalServer;

class CScreenShotObject : QObject
{
    Q_OBJECT
public:
    CScreenShotObject(QObject *parent = Q_NULLPTR);
    ~CScreenShotObject();
    void startScreenShot();
    bool isStart(const QString &account);

private slots:
    void onScreenShotPixmapChanged(const QPixmap &pixmap);

private:
    QLocalServer *m_localServer;
};

#endif // CSCREENSHOTOBJECT_H
