
ZW_FFMPEG_ROOT=/usr/local/opt/ffmpeg

INCLUDEPATH += $${ZW_FFMPEG_ROOT}/include

LIBS += -L$${ZW_FFMPEG_ROOT}/lib -lavcodec -lavfilter -lavdevice -lavutil -lswscale -lavformat -lswresample

