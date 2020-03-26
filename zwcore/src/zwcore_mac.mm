#include "zwcore.h"

#import <AppKit/NSWorkspace.h>

void ZWCore::openFinder(const QString &filePath)
{
    //可以通过NSWorkspace 做更多的操作
    NSString * path    = [NSString stringWithUTF8String:filePath.toUtf8().data()];
    NSURL    * fileURL = [NSURL fileURLWithPath: path];
    NSWorkspace * workspace = [NSWorkspace sharedWorkspace];
    [workspace selectFile:[fileURL path] inFileViewerRootedAtPath:nil];
    return;
}
