
#改成自己本机的QtAV编译好的路径
T_PATH_QTAV_LIB=/Users/miaozw/work/build-QtAV-Desktop_Qt_5_7_1_clang_64bit-Release/lib_osx_x86_64_llvm
#改成自己本机的FFMPEG编译好的路径
T_PATH_FFMPEG=/usr/local/Cellar/ffmpeg/3.3.3

LIBS += -F$${T_PATH_QTAV_LIB}
LIBS += -framework  QtAV
INCLUDEPATH += $${T_PATH_QTAV_LIB}/QtAV.framework/Headers

LIBS += -L$${T_PATH_QTAV_LIB}
LIBS += -lcommon

INCLUDEPATH += $${T_PATH_FFMPEG}/include
LIBS += -L$${T_PATH_FFMPEG}/lib -lavcodec -lavfilter -lavdevice -lavutil -lswscale -lavformat -lswresample


#======以下可选 看实际需求
#QtAVWidgets模块 可选
LIBS += -framework  QtAVWidgets
INCLUDEPATH += $${T_PATH_QTAV_LIB}/QtAVWidgets.framework/Headers

#QmlAV模块 可选
LIBS += -lQmlAV
