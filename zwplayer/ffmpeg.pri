ZW_FFMPEG_PATH=/usr/local/opt/ffmpeg

INCLUDEPATH += $${ZW_FFMPEG_PATH}/include

LIBS += -L$${ZW_FFMPEG_PATH}/lib -lavcodec -lavfilter -lavdevice -lavutil -lswscale -lavformat -lswresample
