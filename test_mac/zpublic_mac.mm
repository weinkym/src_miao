#include "zpublic.h"
#include <QDebug>
#include <QAudioDeviceInfo>
#include <QDataStream>

#import <AVFoundation/AVFoundation.h>
//#import <CoreFoundation/CoreFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMediaIO/CMIOHardware.h>

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
    QList<QByteArray> devices;
    UInt32  propSize = 0;
    AudioObjectPropertyAddress audioDevicesPropertyAddress = { kAudioHardwarePropertyDevices,
                                                               kAudioObjectPropertyScopeGlobal,
                                                               kAudioObjectPropertyElementMaster };

    if (AudioObjectGetPropertyDataSize(kAudioObjectSystemObject,
                                       &audioDevicesPropertyAddress,
                                       0, NULL, &propSize) == noErr) {

        const int dc = propSize / sizeof(AudioDeviceID);
        qDebug()<<"dc="<<dc;

        if (dc > 0)
        {
            AudioDeviceID*  audioDevices = new AudioDeviceID[dc];

            if (AudioObjectGetPropertyData(kAudioObjectSystemObject, &audioDevicesPropertyAddress, 0, NULL, &propSize, audioDevices) == noErr)
            {
                QAudio::Mode mode = QAudio::AudioOutput;
//                QByteArray defaultDevice = (mode == QAudio::AudioOutput) ? defaultOutputDevice() : defaultInputDevice();
                for (int i = 0; i < dc; ++i) {
                    QByteArray info = get_device_info(audioDevices[i], mode);
//                    if (!info.isNull()) {
//                        if (info == defaultDevice)
//                            devices.prepend(info);
//                        else
//                            devices << info;
//                    }
                }
            }

            delete[] audioDevices;
        }
    }
}
