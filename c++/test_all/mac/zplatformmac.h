#ifndef ZPLATFORMMAC_H
#define ZPLATFORMMAC_H
#include <QWidget>

class ZPlatformMac
{
public:
    // 设置窗口显示等级，可以实现显示在最上面
    static void setWidgetLevel(QWidget *w);
    static void setWidgetShadow(QWidget *w,bool hasShadow);
};

#endif // ZPLATFORMMAC_H
