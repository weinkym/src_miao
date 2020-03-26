#-------------------------------------------------
#
# Project created by QtCreator 2016-09-23T15:47:05
#
#-------------------------------------------------

QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zweixin
TEMPLATE = app

DEFINES+=ZWX_NO_WIDGET

include(../zwcore/zwcore.pri)
include(../zwgui/zwgui.pri)

INCLUDEPATH += ./base
INCLUDEPATH += ./network

HEADERS += \
    base/cbaseobject.h \
    base/ccontactmanager.h \
    base/cloginmanager.h \
    base/cmessageinterface.h \
    base/cmodelmap.h \
    base/csqliteaccessinterface.h \
    base/zgolbal.h \
    base/zpeerobjectdata.h \
    base/zpublic.h \
    base/zpublicaction.h \
    network/zrequestaccessmanager.h \
    network/zrequestaction.h

SOURCES += \
    base/cbaseobject.cpp \
    base/ccontactmanager.cpp \
    base/cloginmanager.cpp \
    base/cmessageinterface.cpp \
    base/cmodelmap.cpp \
    base/csqliteaccessinterface.cpp \
    base/zpeerobjectdata.cpp \
    base/zpublic.cpp \
    base/zpublicaction.cpp \
    network/zrequestaccessmanager.cpp \
    network/zrequestaction.cpp

!contains(DEFINES,ZWX_NO_WIDGET):{
message(build with widget)
INCLUDEPATH += ./widget
HEADERS += widget/mainwindow.h \
    widget/zcontackitemwidget.h \
    widget/zcontactlistwidget.h \
    widget/zinputmessagedialog.h \
    widget/zloginwidget.h

SOURCES +=widget/main.cpp \
    widget/mainwindow.cpp \
    widget/zcontackitemwidget.cpp \
    widget/zcontactlistwidget.cpp \
    widget/zinputmessagedialog.cpp \
    widget/zloginwidget.cpp

FORMS += \
    widget/mainwindow.ui \
    widget/zcontackitemwidget.ui \
    widget/zcontactlistwidget.ui \
    widget/zinputmessagedialog.ui \
    widget/zloginwidget.ui
}else{
message(build with no widget)
SOURCES +=main_no_widget.cpp \
}



