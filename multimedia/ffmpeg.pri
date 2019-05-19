
ZW_FFMPEG_ROOT=/usr/local/opt/ffmpeg

INCLUDEPATH += $${ZW_FFMPEG_ROOT}/include

LIBS += -L$${ZW_FFMPEG_ROOT}/lib -lavcodec -lavfilter -lavdevice -lavutil -lswscale -lavformat -lswresample

HEADERS += \
    $$PWD/zwrecordthread.h \
    $$PWD/zwffpublic.h

SOURCES += \
    $$PWD/zwrecordthread.cpp \
    $$PWD/zwffpublic.cpp
