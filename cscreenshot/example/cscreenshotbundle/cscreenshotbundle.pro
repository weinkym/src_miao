#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T18:05:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cscreenshotbundle
TEMPLATE = app

INCLUDEPATH += ../../include
OUTPUT_DIR=$$PWD/TEMP

CONFIG(debug, debug|release) {
    C_LIB_PATH = ../../lib/debug
    C_LIB_NAME = cscreenshotd

    TARGET = $$join(TARGET,,,d)
    DESTDIR = $$OUTPUT_DIR/debug
    DLLDESTDIR = $$LIB_DIR/debug

    OBJECTS_DIR = $$OUTPUT_DIR/debug/cscreenshot/obj
    MOC_DIR = $$OUTPUT_DIR/debug/cscreenshot/moc
    UI_DIR = $$OUTPUT_DIR/debug/cscreenshot/ui
    RCC_DIR = $$OUTPUT_DIR/debug/cscreenshot/rcc
}
CONFIG(release, debug|release) {
    C_LIB_PATH = ../../lib/release
    C_LIB_NAME = cscreenshot
    DESTDIR = $$OUTPUT_DIR/release
    DLLDESTDIR = $$LIB_DIR/release
    OBJECTS_DIR = $$OUTPUT_DIR/release/cscreenshot/obj
    MOC_DIR = $$OUTPUT_DIR/release/cscreenshot/moc
    UI_DIR = $$OUTPUT_DIR/release/cscreenshot/ui
    RCC_DIR = $$OUTPUT_DIR/release/cscreenshot/rcc
}
mac {
    QMAKE_INFO_PLIST = $${TARGET}.plist
}

LIBS += -L$$C_LIB_PATH -l$$C_LIB_NAME

SOURCES += main.cpp\
        widget.cpp \
    cscreenshotobject.cpp

HEADERS  += widget.h \
    cscreenshotobject.h

FORMS    += widget.ui
