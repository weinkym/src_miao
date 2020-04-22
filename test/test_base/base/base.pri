QT *= network svg

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/widgets

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
    $$PWD/widgets/csvgwidget.h

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
    $$PWD/widgets/csvgwidget.cpp
