#-------------------------------------------------
#
# Project created by QtCreator 2016-12-20T15:30:47
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = cwhiteboard
TEMPLATE = lib
DEFINES += CWHITEBOARD_LIBRARY

CGLOBAL_PRF=cglobal.prf
!exists($${CGLOBAL_PRF}) {
    error("$${CGLOBAL_PRF} is not found")
}
include($${CGLOBAL_PRF})


INCLUDEPATH += ./src
INCLUDEPATH += ./include

RESOURCES += \
    source.qrc

HEADERS += \
    src/cdrawitem.h \
    src/cwbpathitem.h \
    src/cwbrectitem.h \
    src/cwhiteboardeditorbar.h \
    src/cwhiteboardeditorbutton.h \
    src/cwhiteboarditem.h \
    src/cwhiteboardpublic.h \
    src/cwhiteboardscene.h \
    src/cwhiteboardview.h \
    include/cwhiteboard_global.h \
    include/cwhiteboard.h \
    src/cwberaseritem.h

SOURCES += \
    src/cdrawitem.cpp \
    src/cwbpathitem.cpp \
    src/cwbrectitem.cpp \
    src/cwhiteboard.cpp \
    src/cwhiteboardeditorbar.cpp \
    src/cwhiteboardeditorbutton.cpp \
    src/cwhiteboarditem.cpp \
    src/cwhiteboardscene.cpp \
    src/cwhiteboardview.cpp \
    src/cwberaseritem.cpp
