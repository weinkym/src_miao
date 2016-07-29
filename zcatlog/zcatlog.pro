#-------------------------------------------------
#
# Project created by QtCreator 2016-01-21T16:51:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zcatlog
TEMPLATE = app

#QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'uiAccess='false'"

SOURCES += main.cpp\
        mainwindow.cpp \
    zcatlogwidget.cpp

HEADERS  += mainwindow.h \
    zcatlogwidget.h

FORMS    += mainwindow.ui \
    zcatlogwidget.ui

    win32: RC_FILE= zcatlog.rc
