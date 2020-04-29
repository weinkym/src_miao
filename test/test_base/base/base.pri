QT *= network svg

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/widgets
INCLUDEPATH += $$PWD/network

include($$PWD/log/log.pri)

HEADERS += \
    $$PWD/cbasewidget.h \
    $$PWD/cbasedialog.h \
    $$PWD/cbasedialogmanager.h \
    $$PWD/widgets/cclickwidget.h \
    $$PWD/cuiutils.h \
    $$PWD/cbaseutils.h \
    $$PWD/cjsonutils.h \
    $$PWD/widgets/csvgwidget.h \
    $$PWD/network/cbaserequestaction.h \
    $$PWD/network/cbaserequester.h \
    $$PWD/network/cdownloadobject.h \
    $$PWD/network/cdownloadrequestaction.h \
    $$PWD/network/cdownloadsettings.h \
    $$PWD/network/crangedata.h \
    $$PWD/network/crequestaccessmanager.h

SOURCES += \
    $$PWD/cbasewidget.cpp \
    $$PWD/cbasedialog.cpp \
    $$PWD/cbasedialogmanager.cpp \
    $$PWD/widgets/cclickwidget.cpp \
    $$PWD/cuiutils.cpp \
    $$PWD/cbaseutils.cpp \
    $$PWD/cjsonutils.cpp \
    $$PWD/widgets/csvgwidget.cpp \
    $$PWD/network/cbaserequestaction.cpp \
    $$PWD/network/cbaserequester.cpp \
    $$PWD/network/cdownloadobject.cpp \
    $$PWD/network/cdownloadrequestaction.cpp \
    $$PWD/network/cdownloadsettings.cpp \
    $$PWD/network/crangedata.cpp \
    $$PWD/network/crequestaccessmanager.cpp
