#ifndef ZWPLATFORMMAC_H
#define ZWPLATFORMMAC_H
#include <QWidget>
#include "zwgui_global.h"

class ZWGUISHARED_EXPORT ZWPlatformMac
{
public:
    // 设置窗口显示等级，可以实现显示在最上面
    static void setWidgetLevel(QWidget *w);

    static void setWidgetShadowable(QWidget *w,bool enable);

    static void setWidgetMiniaturizable(QWidget *w,bool enable);
    static void setWidgetResizable(QWidget *w,bool enable);

    static void doTest();
};

#endif // ZWPLATFORMMAC_H
