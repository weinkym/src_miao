#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T16:49:29
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cscreenshot
include(../include/global.prf)

mac{
QT += macextras
DESTDIR = $$PWD/lib/mac
QMAKE_LFLAGS += -framework AppKit
OBJECTIVE_SOURCES += \
    ./src/cscreenpublic.mm
}

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
    DESTDIR = $$LIB_DIR/debug/cscreenshot
    DLLDESTDIR = $$LIB_DIR/debug/cscreenshot

    OBJECTS_DIR = $$OUTPUT_DIR/debug/cscreenshot/obj
    MOC_DIR = $$OUTPUT_DIR/debug/cscreenshot/moc
    UI_DIR = $$OUTPUT_DIR/debug/cscreenshot/ui
    RCC_DIR = $$OUTPUT_DIR/debug/cscreenshot/rcc
}
CONFIG(release, debug|release) {
    DESTDIR = $$LIB_DIR/release/cscreenshot
    DLLDESTDIR = $$LIB_DIR/release/cscreenshot
    OBJECTS_DIR = $$OUTPUT_DIR/release/cscreenshot/obj
    MOC_DIR = $$OUTPUT_DIR/release/cscreenshot/moc
    UI_DIR = $$OUTPUT_DIR/release/cscreenshot/ui
    RCC_DIR = $$OUTPUT_DIR/release/cscreenshot/rcc
}
win32 {
    LIBS += -luser32
    SOURCES += ./src/cscreenpublic.cpp
}

TEMPLATE = lib

DEFINES += CSCREENSHOT_LIBRARY
INCLUDEPATH += ./include

SOURCES += ./src/cscreenshotmanager.cpp \
    ./src/cscreenattributetoolbaritem.cpp \
    ./src/cscreencolortoolbaritem.cpp \
    ./src/cscreeneditorbuttonitem.cpp \
    ./src/cscreeneditortoolbaritem.cpp \
    ./src/cscreenrectitem.cpp \
    ./src/cscreenselectrectitem.cpp \
    ./src/cscreenshotscene.cpp \
    ./src/cscreenshotview.cpp \
    ./src/cscreentooltipitem.cpp

HEADERS += ./include/cscreenshotmanager.h\
        ./include/cscreenshot_global.h \
    ./include/cscreenattributetoolbaritem.h \
    ./include/cscreencolortoolbaritem.h \
    ./include/cscreeneditorbuttonitem.h \
    ./include/cscreeneditortoolbaritem.h \
    ./include/cscreenpublic.h \
    ./include/cscreenrectitem.h \
    ./include/cscreenselectrectitem.h \
    ./include/cscreenshotscene.h \
    ./include/cscreenshotview.h \
    ./include/cscreentooltipitem.h

RESOURCES += \
    source.qrc

DISTFILES +=

include(test.pri)
