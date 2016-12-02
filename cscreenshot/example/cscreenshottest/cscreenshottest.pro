#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T18:05:09
#
#-------------------------------------------------

#deployqtsss.target = .buildfile
deployqtsss.commands = open "/Users/bitbrothers/Downloads/cscreenshottest.app"
#deployqtsss.commands = @echo Building
QMAKE_EXTRA_TARGETS += deployqtsss

# mytarget.target = .buildfile
#  mytarget.commands = open "/Users/bitbrothers/Downloads/cscreenshottest.app"
#  mytarget.depends = mytarget2

#  mytarget2.commands = open "/Users/bitbrothers/Downloads/cscreenshottest.app"
#QMAKE_EXTRA_TARGETS += mytarget mytarget2


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cscreenshottest
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
    QMAKE_INFO_PLIST = $${TARGET}.plist
}

LIB_PATH=../../lib/$${PLATEFORM_NAME}/$${DEBUG_RELEASE_NAME}
message( "LIB_PATH=$${LIB_PATH}" )
LIBS += -L$${LIB_PATH} -lcscreenshot$${DEBUG_RELEASE_SUBFIX}

#LIBS += -L$$C_LIB_PATH -l$$C_LIB_NAME

SOURCES += main.cpp\
        widget.cpp \
    cwidget.cpp

HEADERS  += widget.h
HEADERS  += cwidget.h

FORMS    += widget.ui \
    cwidget.ui
