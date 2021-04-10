#include "zplatformmac.h"
#include <AppKit/AppKit.h>
#include "zwlog.h"

void ZPlatformMac::setWidgetLevel(QWidget *w)
{
    ZW_LOG_FUNCTION;
    if(w == nullptr)
    {
        return;
    }
    NSView *view = reinterpret_cast<NSView*>(w->winId());
    view.window.level = kCGStatusWindowLevel;//这个等级是snipaste截图钉图的等级
//    view.window.hasShadow = false;
}

void ZPlatformMac::setWidgetShadow(QWidget *w, bool hasShadow)
{
    if(w == nullptr)
    {
        return;
    }
    NSView *view = reinterpret_cast<NSView*>(w->winId());
    view.window.hasShadow = hasShadow;
}
