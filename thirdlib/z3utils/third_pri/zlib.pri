macx: PLATEFORM_NAME = "mac"
else:win32: PLATEFORM_NAME = "win"
else:linux-*: PLATEFORM_NAME = "linux"
else: PLATEFORM_NAME = "unknown"

DEBUG_RELEASE_NAME=release
DEBUG_RELEASE_SUBFIX=
CONFIG(debug, debug|release) {
    DEBUG_RELEASE_NAME=debug
    DEBUG_RELEASE_SUBFIX=d
}

C_LIB_NAME=zwgui$${DEBUG_RELEASE_SUBFIX}
C_LIB_PATH=$$PWD/lib/$${PLATEFORM_NAME}/$${DEBUG_RELEASE_NAME}

C_ROOT_PATH=$$PWD/../../zlib
SOURCES += $${C_ROOT_PATH}/contrib/minizip/zip.c \
    $${C_ROOT_PATH}/contrib/minizip/ioapi.c

HEADERS  += mainwindow.h \
    $${C_ROOT_PATH}/contrib/minizip/zip.h \
    $${C_ROOT_PATH}/contrib/minizip/ioapi.h \
    $${C_ROOT_PATH}/contrib/minizip/crypt.h

#LIBS += -L$${C_LIB_PATH} -l$${C_LIB_NAME}
LIBS += -lz

INCLUDEPATH += $${C_ROOT_PATH}/include
INCLUDEPATH += $${C_ROOT_PATH}/contrib/minizip

message( "C_ROOT_PATH=$${C_ROOT_PATH}" )
#message( "C_LIB_PATH=$${C_LIB_PATH}" )

