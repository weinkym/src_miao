#ifndef _CSCREENSHOTMANAGER_H_
#define _CSCREENSHOTMANAGER_H_

#include <QObject>
#include <QSharedPointer>
#include "cscreenpublic.h"
#include "cscreenshot_global.h"

class CSCREENSHOTSHARED_EXPORT CScreenShotView;

class CSCREENSHOTSHARED_EXPORT CScreenShotManager : public QObject
{
    Q_OBJECT

public:
    static CScreenShotManager *getInstance();
    ~CScreenShotManager();
    void startScreenShot(bool onlySelect);
    void clearAll();

protected:
    explicit CScreenShotManager(QObject *parent=0);

private slots:
    void onStatusChanged(CScreenShotStatus status);
    void onPreviewItemShow();

signals:
    void sigScreenShotPixmapChanged(const QPixmap &pixmap,const QRect &screenRect,const QRect &selectRect);

private:
    static CScreenShotManager *m_instance;
    bool m_isRunning;
    bool m_onlySelect;
    QList<CScreenShotView *> m_viewList;
};

#endif // _CSCREENSHOTMANAGER_H_

