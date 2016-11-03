#-------------------------------------------------
#
# Project created by QtCreator 2016-09-23T15:47:05
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zweixin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    zloginwidget.cpp \
    zrequestaccessmanager.cpp \
    zrequestaction.cpp \
    zpublicaction.cpp \
    zpublic.cpp \
    zpeerobjectdata.cpp

HEADERS  += mainwindow.h \
    zloginwidget.h \
    zrequestaccessmanager.h \
    zrequestaction.h \
    zpublic.h \
    zgolbal.h \
    zpublicaction.h \
    zpeerobjectdata.h

FORMS    += mainwindow.ui \
    zloginwidget.ui
