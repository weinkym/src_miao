#-------------------------------------------------
#
# Project created by QtCreator 2016-08-23T17:09:57
#
#-------------------------------------------------

QT       += core gui

include(../global.prf)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_QSystemTrayIcon
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h
FORMS    += mainwindow.ui

mac {
HEADERS += zdelegate.h
OBJECTIVE_SOURCES += \
znotification.mm \
zdelegate.mm
ICON = kclive.icns
QMAKE_LFLAGS += -framework AppKit
QMAKE_LFLAGS += -framework Foundation
}


