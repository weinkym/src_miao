QT += core

TARGET = copengl
TEMPLATE = app


#SOURCES += main.cpp\
#        widget.cpp

#HEADERS  += widget.h

LIBS += -lkernel32
LIBS += -luser32
LIBS += -lgdi32
LIBS += -lwinspool
LIBS += -lcomdlg32
LIBS += -ladvapi32
LIBS += -lopengl32
LIBS += -lglut32

SOURCES += \
    main.cpp \
    cutilinterface.cpp

HEADERS += \
    cutilinterface.h
