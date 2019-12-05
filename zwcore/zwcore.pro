#-------------------------------------------------
#
# Project created by QtCreator 2017-11-13T09:51:05
#
#-------------------------------------------------

QT       += core gui

TARGET = zwcore
TEMPLATE = lib

CGLOBAL_PRF=cglobal.prf
!exists($${CGLOBAL_PRF}) {
    error("$${CGLOBAL_PRF} is not found")
}
include($${CGLOBAL_PRF})

DEFINES += ZWCORE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $${PWD}/include
HEADERS += include/zwcore_global.h \
    include/zwcore.h \
    include/zwpublic.h \
    include/zwlog.h \
    include/zwlogvalueengine.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SOURCES += \
    src/zwcore.cpp \
    src/zwlog.cpp



macx{
QMAKE_LFLAGS += -framework AppKit
SOURCES += src/zwcore_mac.mm
}
win{
SOURCES += src/zwcore_win.cpp
}

RESOURCES += \
    source.qrc
