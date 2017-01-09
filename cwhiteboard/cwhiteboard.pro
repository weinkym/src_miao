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

TRANSLATIONS += $${TARGET}_zh_CN.ts

INCLUDEPATH += ./src
INCLUDEPATH += ./include

RESOURCES += \
    source.qrc

HEADERS += \
    src/cdrawitem.h \
    src/cwbpathitem.h \
    src/cwbrectitem.h \
    src/cwhiteboarditem.h \
    src/cwhiteboardscene.h \
    include/cwhiteboard_global.h \
    include/cwhiteboard.h \
    include/cwhiteboardview.h \
    include/cwhiteboardeditorbar.h \
    include/cwhiteboardpublic.h \
    src/cwhiteboardeditorbutton.h \
    src/cwberaseritem.h \
    src/cwbaddcommand.h \
    src/cwbclearcommand.h \
    src/cwbtextitem.h \
    src/cwblineitem.h \
    src/cwbpointitem.h \
    src/cwhiteboardviewprivate.h \
    src/cwhiteboardeditorbarprivate.h

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
    src/cwberaseritem.cpp \
    src/cwbaddcommand.cpp \
    src/cwbclearcommand.cpp \
    src/cwbtextitem.cpp \
    src/cwblineitem.cpp \
    src/cwbpointitem.cpp \
    src/cwhiteboardviewprivate.cpp \
    src/cwhiteboardeditorbarprivate.cpp
