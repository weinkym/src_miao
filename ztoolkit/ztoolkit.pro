#-------------------------------------------------
#
# Project created by QtCreator 2014-05-22T09:19:17
#
#-------------------------------------------------
QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET_NAME = ztoolkit
###############################################################################
TARGET = $${TARGET_NAME}
win32: RC_FILE = ztoolkit.rc
###############################################################################
win32 {
CONFIG(debug, debug|release): OBJECTS_DIR = ../../../TEMP/$${TARGET}/debug
CONFIG(release, debug|release): OBJECTS_DIR = ../../../TEMP/$${TARGET}/release
CONFIG(debug, debug|release) : TARGET = $${TARGET_NAME}d
} else {
OBJECTS_DIR = ../../../TEMP/$${TARGET}
}
###############################################################################
UI_DIR      = $${OBJECTS_DIR}/uic
MOC_DIR     = $${OBJECTS_DIR}/moc
RCC_DIR     = $${OBJECTS_DIR}/rcc
INCLUDEPATH += $${OBJECTS_DIR}/uic
###############################################################################
INCLUDEPATH += ./include
INCLUDEPATH += ../mplib/include
###############################################################################
LIBS += -L../mplib/lib -lmplib -luser32
###############################################################################
HEADERS += \
    include/mainwindow.h \
    include/zfiletool.h \
    include/zdeletefiledlg.h \
    include/zrenamefiledlg.h \
    include/zcopyfiledlg.h \
    include/zshutdowndlg.h \
    include/zposcolordlg.h

SOURCES += \
     src/mainwindow.cpp \
     src/zfiletool.cpp \
     src/zdeletefiledlg.cpp \
     src/zrenamefiledlg.cpp \
     src/zcopyfiledlg.cpp \
     src/zshutdowndlg.cpp \
     src/zposcolordlg.cpp \
     src/main.cpp

FORMS += \
    ui/mainwindow.ui \
    ui/zdeletefiledlg.ui \
    ui/zrenamefiledlg.ui \
    ui/zcopyfiledlg.ui \
    ui/zshutdowndlg.ui \
    ui/zposcolordlg.ui

RESOURCES += \
    source.qrc
