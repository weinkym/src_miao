#include <AppKit/AppKit.h>
#include <Foundation/NSUserNotification.h>
#include <Foundation/Foundation.h>
#include <AppKit/NSScreen.h>
#include <AppKit/NSApplication.h>
#include <Foundation/NSString.h>
#include <QString>
#include<QPixmap>
#include <QTemporaryFile>
#include <QImageWriter>
#include "zdelegate.h"


void znotification()
{
    QString message("asdfasdfadf");
    QString title("asdfasdfadf");

    NSUserNotification *notification = [[NSUserNotification alloc] init];
    //当设置“横幅”的情况下，鼠标移动到图片时出现button
    [notification setValue:@YES forKey:@"_showsButtons"];
    //设置是否有提示音,采用系统默认的，自定义的有待研究
    notification.soundName = NSUserNotificationDefaultSoundName;
    //    notification.soundName = @"详细文字说明";

    // QString 转NSString
    //    notification.title = [NSString stringWithUTF8String:title.toUtf8().data()];

    //通过资源文件，产生临时文件加载==================
    QPixmap notificationIconPixmap(":/image/4009_313.png");
    QTemporaryFile notificationIconFile;
    if(notificationIconPixmap.isNull())
    {
        NSLog(@"notificationIconPixmap.isNull()");
        return;
    }
    if(!notificationIconFile.open())
    {
        NSLog(@"!notificationIconFile.open()");
        return;
    }
    QImageWriter writer(&notificationIconFile, "PNG");
    if(!writer.write(notificationIconPixmap.toImage()))
    {
        return;
    }

    //加载本地文件初始化
    NSString *imagePath = [NSString stringWithUTF8String:notificationIconFile.fileName().toUtf8().data()];
    NSImage *image = [[NSImage alloc]initWithContentsOfFile:imagePath];
    //    NSImage *image = [[NSImage alloc]initWithContentsOfFile:@"/Users/bitbrothers/Desktop/background-lecai.png"];
    notification.contentImage = image;
    [image release];

    //imageNamed 无法通过本地文件初始化，只能通过资源文件,但qt资源如何转换有待研究
    //  notification.contentImage = [NSImage imageNamed:@"qrc:/image/4160_138.png"];

    notification.title = @"标题";
    notification.subtitle = @"小标题";
    notification.informativeText = @"详细文字说明";
    notification.hasActionButton = YES;
    notification.actionButtonTitle = @"确定";
//    notification.otherButtonTitle = @"取消";

    //设置通知时间貌似没效果
    //设置通知提交的时间
    notification.deliveryDate = [NSDate dateWithTimeIntervalSinceNow:5];
    //设置通知的循环(必须大于1分钟，估计是防止软件刷屏)
    NSDateComponents *dateComponents = [[NSDateComponents alloc] init];
    [dateComponents setSecond:70];
    notification.deliveryRepeatInterval = dateComponents;
    /**/

    [[NSUserNotificationCenter defaultUserNotificationCenter] deliverNotification:notification];
    //    [[NSUserNotificationCenter defaultUserNotificationCenter] scheduleNotification:notification];

    ZDelegate *delegate = [[ZDelegate alloc] init];
    [[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:delegate];
    return;
}
