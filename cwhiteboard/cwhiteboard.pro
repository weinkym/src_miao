#-------------------------------------------------
#
# Project created by QtCreator 2016-12-20T15:30:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cwhiteboard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cwhiteboarditem.cpp \
    cdrawitem.cpp \
    cwhiteboardview.cpp \
    cwhiteboardscene.cpp

HEADERS  += mainwindow.h \
    cwhiteboarditem.h \
    cdrawitem.h \
    cwhiteboardview.h \
    cwhiteboardscene.h

FORMS    += mainwindow.ui
