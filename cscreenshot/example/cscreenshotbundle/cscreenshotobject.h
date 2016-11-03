#ifndef CSCREENSHOTOBJECT_H
#define CSCREENSHOTOBJECT_H

#include <QObject>

class CScreenShotObject : QObject
{
    Q_OBJECT
public:
    CScreenShotObject(QObject *parent = Q_NULLPTR);
    ~CScreenShotObject();
    void startScreenShot();

private slots:
    void onScreenShotPixmapChanged(const QPixmap &pixmap);
};

#endif // CSCREENSHOTOBJECT_H
