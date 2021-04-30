#include "ZWPlatformMac.h"
#include <AppKit/AppKit.h>
#include "zwlog.h"

static void setWidgetNSWindowStyleMask(QWidget *w,NSWindowStyleMask mask,bool enable)
{
    do{
        if(w == nullptr)
        {
            break;
        }
        NSView *view = reinterpret_cast<NSView*>(w->winId());
        if(view == nullptr)
        {
            break;
        }
        NSWindow* wnd = [view window];

        auto mask_new = [wnd styleMask];
        if(enable)
        {
            mask_new =  mask_new | mask;
        }
        else
        {
            mask_new = mask_new & (mask_new ^ mask);
        }
        ZW_LOG_INFO(QString("styleMask %1 - > %2").arg([wnd styleMask]).arg(mask_new));
        [wnd setStyleMask:mask_new];

    }while(0);
}


void ZWPlatformMac::setWidgetLevel(QWidget *w)
{
    ZW_LOG_FUNCTION;
    if(w == nullptr)
    {
        return;
    }
    NSView *view = reinterpret_cast<NSView*>(w->winId());
    view.window.level = kCGStatusWindowLevel;//这个等级是snipaste截图钉图的等级
//    view.window.hasShadow = false;
    view.window.level = kCGMaximumWindowLevel;
}

void ZWPlatformMac::setWidgetShadowable(QWidget *w, bool enable)
{
    if(w == nullptr)
    {
        return;
    }
    NSView *view = reinterpret_cast<NSView*>(w->winId());
    view.window.hasShadow = enable;
}

void ZWPlatformMac::setWidgetMiniaturizable(QWidget *w, bool enable)
{
    setWidgetNSWindowStyleMask(w,NSWindowStyleMaskMiniaturizable,enable);
}

void ZWPlatformMac::setWidgetResizable(QWidget *w, bool enable)
{
    setWidgetNSWindowStyleMask(w,NSWindowStyleMaskResizable,enable);
}


void ZWPlatformMac::doTest()
{
   [NSApplication sharedApplication];
   [NSApp activateIgnoringOtherApps:YES];
}
