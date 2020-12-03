#-------------------------------------------------
#
# Project created by QtCreator 2019-05-07T14:26:26
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_mac
TEMPLATE = app
CONFIG += C++ 11

#include(QtAV.pri)


#LIBS += -F/Users/miaozw/work/build-QtAV-Desktop_Qt_5_7_1_clang_64bit-Release/lib_osx_x86_64_llvm
#LIBS += -framework  QtAVWidgets

#INCLUDEPATH+=/Users/glodon/Qt5.12.3/5.12.3/clang_64/lib/QtAV.framework/Versions/1/Headers/QtAV
#INCLUDEPATH+=/Users/glodon/Qt5.12.3/5.12.3/clang_64/lib/QtAV.framework/Headers

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(../zwcore/zwcore.pri)

    LIBS += -framework  CoreAudio
    LIBS += -framework  CoreWLAN
    LIBS += -framework  CoreMedia
    LIBS += -framework  AudioToolbox
    LIBS += -framework  AVFoundation
    LIBS += -framework  Foundation
    LIBS += -framework  AppKit
    LIBS += -framework  CFNetwork
    LIBS += -framework  CoreFoundation
    LIBS += -framework  CoreGraphics
    LIBS += -framework  CoreVideo
    LIBS += -framework  IOKit
    LIBS += -framework  OpenGL
    LIBS += -framework  VideoToolbox
    LIBS += -framework  SystemConfiguration
    LIBS += -framework  CoreTelephony
    LIBS += -framework  Accelerate
    LIBS += -framework  CFNetwork

message(INCLUDEPATH=$${INCLUDEPATH})

SOURCES += main.cpp\
        mainwindow.cpp \
    zpublic.cpp \
    zpublic_mac.mm

HEADERS  += mainwindow.h \
    zpublic.h

FORMS    += mainwindow.ui
