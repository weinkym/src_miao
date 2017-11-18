#-------------------------------------------------
#
# Project created by QtCreator 2017-11-15T22:15:35
#
#-------------------------------------------------

TARGET = zwgui
TEMPLATE = lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += ZWGUI_LIBRARY

CGLOBAL_PRF=cglobal.prf
!exists($${CGLOBAL_PRF}) {
    error("$${CGLOBAL_PRF} is not found")
}
include($${CGLOBAL_PRF})
include(../zwcore/zwcore.pri)
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += src/zwgui.cpp \
    src/zwgui_win.cpp \
    src/zwgui_mac.cpp \
    src/zwgui_linux.cpp

INCLUDEPATH += $${PWD}/include
HEADERS += include/zwgui.h\
        include/zwgui_global.h \
    include/zwfilterlistwidget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
