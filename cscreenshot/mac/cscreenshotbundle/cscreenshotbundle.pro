#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T18:05:09
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cscreenshotbundle
TEMPLATE = app

INCLUDEPATH += ../../include

#提供自定义的配置
GLOBAL_PRF=../../screensho_global.prf
exists($${GLOBAL_PRF}) {
    message( "include($${GLOBAL_PRF})" )
    include($${GLOBAL_PRF})
    DESTDIR = $$PWD/$${DEBUG_RELEASE_NAME}
}

mac {
    ICON = $${TARGET}.icns
    QMAKE_INFO_PLIST = $${TARGET}.plist
}

LIB_PATH=../../lib/$${PLATEFORM_NAME}/$${DEBUG_RELEASE_NAME}
message( "LIB_PATH=$${LIB_PATH}" )
LIBS += -L$${LIB_PATH} -lcscreenshot$${DEBUG_RELEASE_SUBFIX}

SOURCES += main.cpp
SOURCES += cscreenshotobject.cpp

HEADERS  += cscreenshotobject.h
