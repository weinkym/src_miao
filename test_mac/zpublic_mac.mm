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

void ZPublic::test()
{
    QList<QPair<QVariant, QVariant> > objList;
    for (AVCaptureDevice *dev in [AVCaptureDevice
         devices]) {
        if ([dev hasMediaType: AVMediaTypeVideo] ||
                [dev hasMediaType: AVMediaTypeMuxed])
        {
//            AVCaptureDALDevice;
//            objList.append(QPair<QVariant, QVariant>(dev.localizedName.UTF8String,dev.uniqueID.UTF8String));
//            qDebug()<<dev.localizedName.UTF8String<<" ======  "<<dev.uniqueID.UTF8String;
//            qDebug()<<dev.uniqueID.UTF8String;
//            qDebug()<<dev.modelID.UTF8String;
//            obs_property_list_add_string(dev_list,
//                                         dev.localizedName.UTF8String,
//                                         dev.uniqueID.UTF8String);
        }
//        if ([dev hasMediaType: AVMediaTypeAudio])
        {
            NSString *name = [NSString stringWithUTF8String:object_getClassName(dev)];
            ZW_LOG_INFO(ZW_LOG_P(name.UTF8String));

//            AVCaptureDeviceType *type = dev.deviceType.UTF8String;
//            dev.deviceType.UTF8String;
            QString content = ZW_LOG_P6(dev.localizedName.UTF8String,dev.uniqueID.UTF8String,dev.modelID.UTF8String,
                      dev.manufacturer.UTF8String,dev.transportType,dev.manufacturer.UTF8String);
            ZW_LOG_INFO(content);
//            objList.append(QPair<QVariant, QVariant>(dev.localizedName.UTF8String,dev.uniqueID.UTF8String));
//            qDebug()<<"===========>>"<<dev.localizedName.UTF8String<<" ======  "<<dev.uniqueID.UTF8String
//                   <<dev.modelID.UTF8String<<dev.manufacturer.UTF8String
//                   <<dev.transportType<<dev.manufacturer.UTF8String;
//            qDebug()<<dev.uniqueID.UTF8String;
//            qDebug()<<dev.modelID.UTF8String;
//            obs_property_list_add_string(dev_list,
//                                         dev.localizedName.UTF8String,
//                                         dev.uniqueID.UTF8String);
        }
    }

    return;
}
