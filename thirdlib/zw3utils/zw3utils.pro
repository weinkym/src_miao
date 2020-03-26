#-------------------------------------------------
#
# Project created by QtCreator 2018-01-11T17:35:55
#
#-------------------------------------------------

QT       -= gui

TARGET = zw3utils
TEMPLATE = lib

DEFINES += ZW3UTILS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CGLOBAL_PRF=cglobal.prf
!exists($${CGLOBAL_PRF}) {
    error("$${CGLOBAL_PRF} is not found")
}
include($${CGLOBAL_PRF})
include(./third_pri/zlib.pri)

include(../../zwcore/zwcore.pri)

INCLUDEPATH += $${PWD}/include

SOURCES += src/zw3utils.cpp

HEADERS += include/zw3utils.h\
        include/zw3utils_global.h


