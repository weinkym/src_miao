#include "zdelegate.h"
void testCallBack()
{
    NSLog(@"testCallBack");
}

@implementation ZDelegate
- (void)userNotificationCenter:(NSUserNotificationCenter *)center didDeliverNotification:(NSUserNotification *)notification
{
    NSLog(@"通知已经递交！");
    testCallBack();
}

- (void)userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification
{
    NSLog(@"用户点击了通知！");
    testCallBack();
}

- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification
{
    //用户中心决定不显示该条通知(如果显示条数超过限制或重复通知等)，returen YES;强制显示
//    return NO;
    testCallBack();
    return YES;
}
@end
