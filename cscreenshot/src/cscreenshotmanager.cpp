#include <QApplication>
#include <QGuiApplication>
#include "cscreenshotmanager.h"
#include "cscreenshotview.h"
#include "cscreenpublic.h"
#include <QDebug>
#include <QWindowList>
#include <QWindow>
#include "cscreenshottimer.h"

CScreenShotManager * CScreenShotManager::m_instance = NULL;

CScreenShotManager::CScreenShotManager(QObject *parent)
    :QObject(parent)
    ,m_isRunning(false)
    ,m_onlySelect(false)
{
}

CScreenShotManager *CScreenShotManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CScreenShotManager;
    }
    return m_instance;
}

CScreenShotManager::~CScreenShotManager()
{
    clearAll();
}

void CScreenShotManager::startScreenShot(bool onlySelect)
{
    C_SCREENSHOT_LOG_FUNCTION;
    if(m_isRunning)
    {
        return;
    }
    C_SCREENSHOT_LOG_TEST;
    clearAll();
    C_SCREENSHOT_LOG_TEST;
    m_onlySelect = onlySelect;
    QList<QScreen *> screens = QApplication::screens();
    int index = 0;
    QList<QRect> rectList = getWindownRectList();
    C_SCREENSHOT_LOG_INFO(QString("all rect count %1").arg(rectList.count()));

    foreach (QScreen *d, screens)
    {
        index++;
        C_SCREENSHOT_LOG_TEST;
        CScreenShotView *view = new CScreenShotView(rectList,d,onlySelect);
        m_viewList.append(view);
        connect(view,SIGNAL(sigStatusChanged(CScreenShotStatus)),
                this,SLOT(onStatusChanged(CScreenShotStatus)));
        connect(view,SIGNAL(sigPreviewItemShow()),
                this,SLOT(onPreviewItemShow()));
        view->startSCreenShot();
        C_SCREENSHOT_LOG_TEST;
        view->raise();
        C_SCREENSHOT_LOG_TEST;
    }
    m_isRunning = true;
    C_SCREENSHOT_LOG_TEST;
}

void CScreenShotManager::clearAll()
{
    C_SCREENSHOT_LOG_FUNCTION;
    //可能为导致crash
    //qDeleteAll(m_viewList);
    foreach (CScreenShotView *v, m_viewList)
    {
        v->setVisible(false);
        v->deleteLater();
    }
    m_viewList.clear();
    C_SCREENSHOT_LOG_TEST;
}

void CScreenShotManager::onStatusChanged(CScreenShotStatus status)
{
    CScreenShotView *view = dynamic_cast<CScreenShotView*>(sender());
    if(view == NULL)
    {
        C_SCREENSHOT_LOG_INFO(QString("view is NULL"));
        return;
    }

    QPixmap pixmap;
    QRect screenRect = view->getScreenGeometry();
    QRect selectRect = view->getSelectRect().toRect();

    bool isValid = false;
    C_SCREENSHOT_LOG_TEST;
    if(status == CSCREEN_SHOT_STATE_FINISHED)
    {
        pixmap = view->getPixmap();
        isValid = view->isValid();
    }
    C_SCREENSHOT_LOG_TEST;

    CScreenShotView *firstView = NULL;
    foreach (CScreenShotView *d, m_viewList)
    {
        if(status == CSCREEN_SHOT_STATE_CANCEL || status == CSCREEN_SHOT_STATE_FINISHED)
        {
            if(firstView != NULL)
            {
                d->deleteLater();
            }
            else
            {
                firstView = d;
            }
        }
        else
        {
            if(view != d)
            {
                d->setLocked(true);
            }
        }
    }
    C_SCREENSHOT_LOG_TEST;

    if(status == CSCREEN_SHOT_STATE_CANCEL || status == CSCREEN_SHOT_STATE_FINISHED)
    {
        if(firstView)
        {
            firstView->deleteLater();
        }
        m_viewList.clear();
        C_SCREENSHOT_LOG_TEST;
        if(status == CSCREEN_SHOT_STATE_FINISHED)
        {
            C_SCREENSHOT_LOG_TEST;
            if(!pixmap.isNull() && isValid)
            {
                C_SCREENSHOT_LOG_TEST;
                emit sigScreenShotPixmapChanged(pixmap,screenRect,selectRect);
            }
            else
            {
                C_SCREENSHOT_LOG_INFO(QString("shot is %1valid,pixmap is %2null")
                                       .arg(isValid?"":"not")
                                       .arg(pixmap.isNull()?"":"not "));
            }
        }
        m_isRunning = false;
    }
    C_SCREENSHOT_LOG_TEST;
}

void CScreenShotManager::onPreviewItemShow()
{
    CScreenShotView *view = dynamic_cast<CScreenShotView*>(sender());
    if(view == NULL)
    {
        C_SCREENSHOT_LOG_INFO(QString("view is NULL"));
        return;
    }
    foreach (CScreenShotView *d, m_viewList)
    {
        if(d != view)
        {
            d->setPreviewItemHidden(true);
        }
    }
}
