#include "zpublic.h"
#include <QDebug>
#include <QAudioDeviceInfo>
#include <QDataStream>

#import <AVFoundation/AVFoundation.h>
//#import <CoreFoundation/CoreFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMediaIO/CMIOHardware.h>
#import <CoreServices/CoreServices.h>

QList<QPair<QVariant, QVariant> > ZPublic::getCameras()
{
    QList<QPair<QVariant, QVariant> > objList;
    for (AVCaptureDevice *dev in [AVCaptureDevice
         devices]) {
        if ([dev hasMediaType: AVMediaTypeVideo] ||
                [dev hasMediaType: AVMediaTypeMuxed])
        {
//            objList.append(QPair<QVariant, QVariant>(dev.localizedName.UTF8String,dev.uniqueID.UTF8String));
            qDebug()<<dev.localizedName.UTF8String<<" ======  "<<dev.uniqueID.UTF8String;
//            qDebug()<<dev.uniqueID.UTF8String;
//            qDebug()<<dev.modelID.UTF8String;
//            obs_property_list_add_string(dev_list,
//                                         dev.localizedName.UTF8String,
//                                         dev.uniqueID.UTF8String);
        }
        if ([dev hasMediaType: AVMediaTypeAudio])
        {
            objList.append(QPair<QVariant, QVariant>(dev.localizedName.UTF8String,dev.uniqueID.UTF8String));
//            qDebug()<<dev.localizedName.UTF8String<<" ======  "<<dev.uniqueID.UTF8String
//                   <<dev.modelID.UTF8String<<dev.manufacturer.UTF8String
//                   <<dev.transportType<<dev.manufacturer.UTF8String;
//            qDebug()<<dev.uniqueID.UTF8String;
//            qDebug()<<dev.modelID.UTF8String;
//            obs_property_list_add_string(dev_list,
//                                         dev.localizedName.UTF8String,
//                                         dev.uniqueID.UTF8String);
        }
    }

    return objList;
}

void ZPublic::getVersion(SInt32 &major, SInt32 &minor, SInt32 &bugfix)
{
//    SInt32 major, minor, bugfix;
    Gestalt(gestaltSystemVersionMajor, &major);
    Gestalt(gestaltSystemVersionMinor, &minor);
    Gestalt(gestaltSystemVersionBugFix, &bugfix);
}


static inline QString cfStringToQString(CFStringRef str)
{
    CFIndex length = CFStringGetLength(str);
    const UniChar *chars = CFStringGetCharactersPtr(str);
    if (chars)
        return QString(reinterpret_cast<const QChar *>(chars), length);

    UniChar buffer[length];
    CFStringGetCharacters(str, CFRangeMake(0, length), buffer);
    return QString(reinterpret_cast<const QChar *>(buffer), length);
}

static QByteArray get_device_info(AudioDeviceID audioDevice, QAudio::Mode mode)
{
    UInt32      size;
    QByteArray  device;
    QDataStream ds(&device, QIODevice::WriteOnly);
    AudioStreamBasicDescription     sf;
    CFStringRef name;
    Boolean     isInput = mode == QAudio::AudioInput;
    AudioObjectPropertyScope audioPropertyScope = isInput ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;

    // Id
    ds << quint32(audioDevice);
    qDebug()<<quint32(audioDevice);

    // Mode //TODO: Why don't we use the Stream Format we ask for?
    size = sizeof(AudioStreamBasicDescription);
    AudioObjectPropertyAddress audioDeviceStreamFormatPropertyAddress = { kAudioDevicePropertyStreamFormat,
                                                                    audioPropertyScope,
                                                                    kAudioObjectPropertyElementMaster };

//    if (AudioObjectGetPropertyData(audioDevice, &audioDeviceStreamFormatPropertyAddress, 0, NULL, &size, &sf) != noErr) {
//        return QByteArray();
//    }

//    qDebug()<<"line="<<__LINE__;

//    kAudioObjectPropertyBaseClass           = 'bcls',
//    kAudioObjectPropertyClass               = 'clas',
//    kAudioObjectPropertyOwner               = 'stdv',
//    kAudioObjectPropertyName                = 'lnam',
//    kAudioObjectPropertyModelName           = 'lmod',
//    kAudioObjectPropertyManufacturer        = 'lmak',
//    kAudioObjectPropertyElementName         = 'lchn',
//    kAudioObjectPropertyElementCategoryName = 'lccn',
//    kAudioObjectPropertyElementNumberName   = 'lcnn',
//    kAudioObjectPropertyOwnedObjects        = 'ownd',
//    kAudioObjectPropertyIdentify            = 'iden',
//    kAudioObjectPropertySerialNumber        = 'snum',
//    kAudioObjectPropertyFirmwareVersion     = 'fwvn'

    // Name
    size = sizeof(CFStringRef);
    AudioObjectPropertyAddress audioDeviceNamePropertyAddress = { kAudioObjectPropertyName,
                                                                  audioPropertyScope,
                                                                  kAudioObjectPropertyElementMaster };
//    AudioObjectPropertyAddress audioDeviceNamePropertyAddress = { kAudioObjectPropertyName,
//                                                                  audioPropertyScope,
//                                                                  kAudioObjectPropertyElementMaster };

    if (AudioObjectGetPropertyData(audioDevice, &audioDeviceNamePropertyAddress, 0, NULL, &size, &name) != noErr) {
        qWarning() << "QAudioDeviceInfo: Unable to find device name";
        return QByteArray();
    }
    ds << cfStringToQString(name);

    CFRelease(name);

    qDebug()<<cfStringToQString(name)<<quint32(audioDevice);

    return device;
}

qreal getGB(unsigned long value)
{
    return value / 1024.0 / 1024.0 / 1024.0;
}

void ZPublic::test()
{
    mach_port_t host_port;
    mach_msg_type_number_t host_size;
    vm_size_t pagesize;

    host_port = mach_host_self();
    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    host_page_size(host_port, &pagesize);

    vm_statistics_data_t vm_stat;

    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS)
        NSLog(@"Failed to fetch vm statistics");

    /* Stats in bytes */
    unsigned long mem_used = (vm_stat.active_count +
                          vm_stat.inactive_count +
                          vm_stat.wire_count) * (unsigned long)pagesize;
    unsigned long mem_free = vm_stat.free_count * (unsigned long)pagesize;
    unsigned long mem_total = (unsigned long)mem_used + (unsigned long)mem_free;
    NSLog(@"used: %ld free: %ld total: %ld pageSize: %ld", mem_used, mem_free, mem_total, pagesize);

    qDebug()<<"OOOOO==="<<getGB(mem_used)<<getGB(mem_free)<<getGB(mem_total)<<pagesize;

    return;
}

