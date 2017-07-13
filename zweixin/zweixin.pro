#-------------------------------------------------
#
# Project created by QtCreator 2016-09-23T15:47:05
#
#-------------------------------------------------

QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zweixin
TEMPLATE = app

INCLUDEPATH += ./base
INCLUDEPATH += ./widget
INCLUDEPATH += ./network

SOURCES += main.cpp\
        mainwindow.cpp \
    zloginwidget.cpp \
    zrequestaccessmanager.cpp \
    zrequestaction.cpp \
    zpublicaction.cpp \
    zpublic.cpp \
    zpeerobjectdata.cpp \
    zcontackitemwidget.cpp \
    base/ccontactmanager.cpp \
    base/cbaseobject.cpp \
    base/cloginmanager.cpp \
    base/cmessageinterface.cpp \
    base/csqliteaccessinterface.cpp

HEADERS  += mainwindow.h \
    zloginwidget.h \
    zrequestaccessmanager.h \
    zrequestaction.h \
    zpublic.h \
    zgolbal.h \
    zpublicaction.h \
    zpeerobjectdata.h \
    zcontackitemwidget.h \
    base/ccontactmanager.h \
    base/cbaseobject.h \
    base/cloginmanager.h \
    base/cmessageinterface.h \
    base/csqliteaccessinterface.h

FORMS    += mainwindow.ui \
    zloginwidget.ui \
    zcontackitemwidget.ui
