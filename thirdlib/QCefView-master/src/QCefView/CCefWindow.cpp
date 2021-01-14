#pragma region qt_headers
#include <QCoreApplication>
#include <QDebug>
#include <QPaintDevice>
#include <QPainter>
#include <QResizeEvent>
#pragma endregion qt_headers

#include "CCefManager.h"
#include "CCefWindow.h"

#define CEF_BROWSER_WINDOW_CLASS_NAME_A "CefBrowserWindow"

QHash<CefWindowHandle, CCefWindow *> CCefWindow::instanceMap_;

QMutex CCefWindow::instanceMtx_;

CCefWindow *
CCefWindow::lookupInstance(CefWindowHandle wnd)
{
    QMutexLocker locker(&instanceMtx_);
    return instanceMap_[wnd];
}

CCefWindow::CCefWindow(QWindow *parent /*= 0*/)
    : QWindow(parent)
    , hwndCefBrowser_(nullptr)
{
    setFlags(Qt::FramelessWindowHint);

    CCefManager::getInstance().initializeCef();
}

CCefWindow::~CCefWindow()
{
    if(hwndCefBrowser_)
        hwndCefBrowser_ = nullptr;

    CCefManager::getInstance().uninitializeCef();
}

void CCefWindow::setCefBrowserWindow(CefWindowHandle wnd)
{
    hwndCefBrowser_ = wnd;
    syncCefBrowserWindow();

    {
        QMutexLocker locker(&instanceMtx_);
        instanceMap_[wnd] = this;
    }
}

void CCefWindow::syncCefBrowserWindow()
{
    double w = width() * devicePixelRatio();
    double h = height() * devicePixelRatio();

#ifdef ZW_OS_WIN
    if(hwndCefBrowser_)
        ::SetWindowPos(hwndCefBrowser_, NULL, 0, 0, w, h, SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_DEFERERASE);
#else
#endif
}

void CCefWindow::exposeEvent(QExposeEvent *e)
{
    syncCefBrowserWindow();
#ifdef ZW_OS_WIN
    return __super::exposeEvent(e);
#else
#endif
}

void CCefWindow::resizeEvent(QResizeEvent *e)
{

    syncCefBrowserWindow();
#ifdef ZW_OS_WIN
    __super::resizeEvent(e);
#else
#endif

    return;
}
