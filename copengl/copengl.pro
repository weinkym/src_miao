QT += core

TARGET = copengl
TEMPLATE = app


#SOURCES += main.cpp\
#        widget.cpp

#HEADERS  += widget.h
win32{
LIBS += -lkernel32
LIBS += -luser32
LIBS += -lgdi32
LIBS += -lwinspool
LIBS += -lcomdlg32
LIBS += -ladvapi32
LIBS += -lopengl32
LIBS += -lglut32
}
mac{
    LIBS += -framework  OpenGL
    LIBS += -framework  GLUT
    LIBS += -framework  Foundation
INCLUDEPATH += /usr/local/Cellar/glm/0.9.8.4/include
#LIBS+= -L/usr/local/Cellar/glm/0.9.8.4/lib -lglm
INCLUDEPATH += /usr/local/Cellar/sdl/1.2.15/include
LIBS+= -L/usr/local/Cellar/sdl/1.2.15/lib -lsdl

}



SOURCES += \
    main.cpp \
    cutilinterface.cpp \
    mesh.cpp \
    display.cpp \
    shader.cpp \
    include/LoadShaders.cpp \
    texture.cpp

HEADERS += \
    cutilinterface.h \
    mesh.h \
    display.h \
    shader.h \
    include/LoadShaders.h \
    texture.h
