QT *= network svg

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/widgets
INCLUDEPATH += $$PWD/download

include($$PWD/log/log.pri)

HEADERS += \
    $$PWD/cbaserequester.h \
    $$PWD/cbaserequestaction.h \
    $$PWD/cbasewidget.h \
    $$PWD/cbasedialog.h \
    $$PWD/cbasedialogmanager.h \
    $$PWD/crequestaccessmanager.h \
    $$PWD/widgets/cclickwidget.h \
    $$PWD/cuiutils.h \
    $$PWD/cbaseutils.h \
    $$PWD/cjsonutils.h \
    $$PWD/widgets/csvgwidget.h \
    $$PWD/download/cdownloadobject.h \
    $$PWD/download/cdownloadsettings.h \
    $$PWD/download/crangedata.h \
    $$PWD/download/cdownloadrequestaction.h

SOURCES += \
    $$PWD/cbaserequester.cpp \
    $$PWD/cbaserequestaction.cpp \
    $$PWD/cbasewidget.cpp \
    $$PWD/cbasedialog.cpp \
    $$PWD/cbasedialogmanager.cpp \
    $$PWD/crequestaccessmanager.cpp \
    $$PWD/widgets/cclickwidget.cpp \
    $$PWD/cuiutils.cpp \
    $$PWD/cbaseutils.cpp \
    $$PWD/cjsonutils.cpp \
    $$PWD/widgets/csvgwidget.cpp \
    $$PWD/download/cdownloadobject.cpp \
    $$PWD/download/cdownloadsettings.cpp \
    $$PWD/download/crangedata.cpp \
    $$PWD/download/cdownloadrequestaction.cpp
