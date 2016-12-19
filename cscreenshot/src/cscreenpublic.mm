#include "cscreenpublic.h"
#include <QtMac>
#include <QScreen>
#include <QPixmap>
#import <Foundation/Foundation.h>
#import <AppKit/NSScreen.h>
#import <AppKit/NSApplication.h>

QList<QRect> getWindownRectList()
{
    QList<QRect> rectList;
    //以下注释代码为探索性，暂时保留
    //    NSArray *screenArray = [NSScreen screens];
    //    NSScreen *mainScreen = [NSScreen mainScreen];
    //    unsigned screenCount = [screenArray count];
    //    unsigned index  = 0;
    //    NSLog(@"screenCount #%d", screenCount);

    //    for (index; index < screenCount; index++)
    //    {
    //      NSScreen *screen = [screenArray objectAtIndex: index];
    //      NSRect screenRect = [screen visibleFrame];
    //      screenRect = [screen convertRectToBacking: screenRect];
    //      NSString *mString = ((mainScreen == screen) ? @"Main" : @"not-main");

    //      NSLog(@"Screen #%d (%@) Frame: %@", index, mString, NSStringFromRect(screenRect));
    //    }
    //    int windowCount = [[[NSApplication sharedApplication] windows] count];
    //    NSLog(@"windowCount #%d", windowCount);

    NSMutableArray *windows = (__bridge NSMutableArray *)CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements, kCGNullWindowID);

    for (NSDictionary *entry in windows)
    {
        CGRect bounds;
        //                CGRectMakeWithDictionaryRepresentation((CFDictionaryRef)entry[(id)kCGWindowBounds], &bounds);
        CGRectMakeWithDictionaryRepresentation((CFDictionaryRef)[entry objectForKey:@"kCGWindowBounds"], &bounds);

//        NSString *applicationName = entry[(id)kCGWindowOwnerName];
//        if(applicationName != NULL)
//        {
//            // PID is required so we assume it's present.
//            NSString *nameAndPID = [NSString stringWithFormat:@"%@ (%@)", applicationName, entry[(id)kCGWindowOwnerPID]];
//            NSLog(@"rect(%0.0f,%0.0f,%0.0f,%0.0f),nameAndPID=%@,name=%@",bounds.origin.x,bounds.origin.y,bounds.size.width,bounds.size.height,nameAndPID,applicationName);
//        }
//        else
//        {
//            // The application name was not provided, so we use a fake application name to designate this.
//            // PID is required so we assume it's present.
//            NSString *nameAndPID = [NSString stringWithFormat:@"((unknown)) (%@)", entry[(id)kCGWindowOwnerPID]];
//            NSLog(@"rect(%0.0f,%0.0f,%0.0f,%0.0f),nameAndPID=%@",bounds.origin.x,bounds.origin.y,bounds.size.width,bounds.size.height,nameAndPID);
//        }
        QRect rect(bounds.origin.x,bounds.origin.y,bounds.size.width,bounds.size.height);
        if(!rectList.contains(rect))
        {
            rectList.append(rect);
        }
    }
    return rectList;
}

QPixmap getScreenPixmap(const QScreen *screen)
{
    if(screen == NULL)
    {
        return QPixmap();
    }
    QRect rect = screen->geometry();
    CGRect captureRect;
    captureRect.origin.x = rect.x();
    captureRect.origin.y = rect.y();
    captureRect.size.width = rect.width();
    captureRect.size.height = rect.height();

    CGImageRef screenShot = CGWindowListCreateImage(captureRect, kCGWindowListOptionOnScreenOnly, kCGNullWindowID, kCGWindowImageDefault);
    //全部
    //    CGImageRef screenShot = CGWindowListCreateImage(CGRectInfinite, kCGWindowListOptionOnScreenOnly, kCGNullWindowID, kCGWindowImageDefault);
    QPixmap pixmap= QtMac::fromCGImageRef(screenShot);
    return pixmap;
}
