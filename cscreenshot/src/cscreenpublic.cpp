#include "cscreenpublic.h"
#include <QPair>
QList<QRect> gWindownRectList; //用于存储所有的窗口

#ifndef C_SCREENSHOT_TEST_CODE
//#define C_SCREENSHOT_TEST_CODE
#endif

#ifdef C_SCREENSHOT_TEST_CODE
QList<QPair<QString,unsigned long> > gTypeList; //用于存储所有的窗口
QList<unsigned long> existList;
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#include <vector>
//声明回调函数
bool CALLBACK getWindownRect(HWND hwnd,LPARAM lParam);

bool CALLBACK getWindownRect(HWND hWnd,LPARAM lParam)
{
    DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

    //WS_CLIPCHILDREN样式主要是用于父窗口，也就是说当在父窗口绘制的时候，父窗口上还有一个子窗口，那么设置了这个样式的话，子窗口区域父窗口就不负责绘制。
    if(!(dwStyle & WS_CLIPCHILDREN))
    {
        return true;
    }

    if((dwStyle & WS_VISIBLE) != WS_VISIBLE)
    {
        return TRUE;
    }

    DWORD dwStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
    DWORD dwStyleMustNot = WS_EX_TRANSPARENT;
    if((dwStyleMustNot & dwStyleEx) != 0)
    {
        return TRUE;
    }

    if (IsWindow(hWnd) && IsWindowVisible(hWnd) && !IsIconic(hWnd))
    {
        RECT tempRect;
        QRect rect;
        ::GetWindowRect(hWnd,&tempRect);

//        ::GetClientRect(hWnd,&tempRect);
//        LPPOINT point = (LPPOINT)malloc(sizeof(tagPOINT));
//        point->x = tempRect.left;
//        point->y = tempRect.top;
//        ::ClientToScreen(hWnd,point);
//        rect=QRect(point->x,point->y,tempRect.right - tempRect.left,tempRect.bottom - tempRect.top);

        rect.setTopLeft(QPoint(tempRect.left,tempRect.top));
        //QRect 差一个px
        rect.setBottomRight(QPoint(tempRect.right - 1,tempRect.bottom - 1));
        if(!gWindownRectList.contains(rect))
        {
            gWindownRectList.append(rect);
        }
        //遍历子窗口,TODO
        //  ::EnumChildWindows(hwnd,(WNDENUMPROC)updateWindowRectList,0);
#ifdef C_SCREENSHOT_TEST_CODE
        if(!existList.contains(dwStyle))
        {
            existList.append(dwStyle);
            QString testString;
            QPair<QString,unsigned long> tempV;
            //         if(dwStyle == 2483027968 || dwStyle == 2496593920)
            testString.append(QString("%1=0;").arg(dwStyle));
            foreach (tempV, gTypeList)
            {
                testString.append(QString("%1=%2;").arg(tempV.first).arg((tempV.second & dwStyle) == 0 ? "0":"1"));
            }
            qDebug()<<testString<<rect;
        }
#endif
    }
    return true;
}
#endif

QList<QRect> getWindownRectList()
{
#ifdef C_SCREENSHOT_TEST_CODE
    existList.clear();
    gTypeList.clear();
    gTypeList.append(QPair<QString,unsigned long>("WS_OVERLAPPED",WS_OVERLAPPED));
    gTypeList.append(QPair<QString,unsigned long>("WS_OVERLAPPED",WS_OVERLAPPED));
    gTypeList.append(QPair<QString,DWORD>("WS_POPUP",WS_POPUP));
    gTypeList.append(QPair<QString,DWORD>("WS_CHILD",WS_CHILD));
    gTypeList.append(QPair<QString,DWORD>("WS_MINIMIZE",WS_MINIMIZE));
    gTypeList.append(QPair<QString,DWORD>("WS_VISIBLE",WS_VISIBLE));
    gTypeList.append(QPair<QString,DWORD>("WS_DISABLED",WS_DISABLED));
    gTypeList.append(QPair<QString,DWORD>("WS_CLIPSIBLINGS",WS_CLIPSIBLINGS));
    gTypeList.append(QPair<QString,DWORD>("WS_CLIPCHILDREN",WS_CLIPCHILDREN));
    gTypeList.append(QPair<QString,DWORD>("WS_MAXIMIZE",WS_MAXIMIZE));
    gTypeList.append(QPair<QString,DWORD>("WS_CAPTION",WS_CAPTION));
    gTypeList.append(QPair<QString,DWORD>("WS_BORDER",WS_BORDER));
    gTypeList.append(QPair<QString,DWORD>("WS_DLGFRAME",WS_DLGFRAME));
    gTypeList.append(QPair<QString,DWORD>("WS_VSCROLL",WS_VSCROLL));
    gTypeList.append(QPair<QString,DWORD>("WS_HSCROLL",WS_HSCROLL));
    gTypeList.append(QPair<QString,DWORD>("WS_SYSMENU",WS_SYSMENU));
    gTypeList.append(QPair<QString,DWORD>("WS_THICKFRAME",WS_THICKFRAME));
    gTypeList.append(QPair<QString,DWORD>("WS_GROUP",WS_GROUP));
    gTypeList.append(QPair<QString,DWORD>("WS_TABSTOP",WS_TABSTOP));
    gTypeList.append(QPair<QString,DWORD>("WS_OVERLAPPED",WS_OVERLAPPED));
#endif

    gWindownRectList.clear();
    //遍历所有顶级窗口,更新窗口大小列表
    ::EnumWindows((WNDENUMPROC)getWindownRect,0);
    return gWindownRectList;
}
