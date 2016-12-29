macx: PLATEFORM_NAME = "mac"
else:win32: PLATEFORM_NAME = "win"
else:linux-*: PLATEFORM_NAME = "linux-$${ARCHITECTURE}"
else: PLATEFORM_NAME = "unknown"

DEBUG_RELEASE_NAME=release
DEBUG_RELEASE_SUBFIX=
CONFIG(debug, debug|release) {
    DEBUG_RELEASE_NAME=debug
    DEBUG_RELEASE_SUBFIX=d
}

C_LIB_NAME=mplib$${DEBUG_RELEASE_SUBFIX}
C_LIB_PATH=$$PWD/lib/$${PLATEFORM_NAME}/$${DEBUG_RELEASE_NAME}

LIBS += -L$${C_LIB_PATH} -l$${C_LIB_NAME}

INCLUDEPATH += $$PWD/include
message( "C_LIB_NAME=$${C_LIB_NAME}" )
message( "C_LIB_PATH=$${C_LIB_PATH}" )

