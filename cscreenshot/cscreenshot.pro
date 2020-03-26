#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T16:49:29
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cscreenshot

#提供自定义的配置
GLOBAL_PRF=screensho_global.prf
exists($${GLOBAL_PRF}) {
    message( "include($${GLOBAL_PRF})" )
    include($${GLOBAL_PRF})
    DESTDIR = $$PWD/lib/$${PLATEFORM_NAME}/$${DEBUG_RELEASE_NAME}
}

include(../include/global.prf)

mac{
QT += macextras
#DESTDIR = $$PWD/lib/mac
QMAKE_LFLAGS += -framework AppKit
OBJECTIVE_SOURCES += \
    ./src/cscreenpublic.mm
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
    ./src/cscreentooltipitem.cpp \
    ./src/cscreenshotutil.cpp \
    ./src/cscreenshottimer.cpp

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
    ./include/cscreentooltipitem.h \
    include/cscreenshotutil.h \
    include/cscreenshottimer.h

RESOURCES += \
    source.qrc

DISTFILES +=
