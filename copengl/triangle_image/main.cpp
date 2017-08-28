#ifdef Q_OS_WIN
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#include <QDebug>
#include "mesh.h"
#include "shader.h"

#include "texture.h"

Mesh *g_mesh = 0;
Shader *g_shader = 0;
Texture *g_texture = 0;

void myDisplay(void)
{
    int i;
    glClear(GL_COLOR_BUFFER_BIT);
//    glLineWidth(3);
//    glPointSize(3);
    g_shader->bind();
    g_texture->Bind();
    g_mesh->draw();
    glFlush();
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow(QString("第一个OpenGL程序").toLocal8Bit().data());
    const  GLubyte * name = glGetString(GL_VENDOR);
    const  GLubyte * biaoshifu = glGetString(GL_RENDERER);
    const  GLubyte * OpenGLVersion = glGetString(GL_VERSION);
    const  GLubyte * gluVersion = gluGetString(GLU_VERSION);
    qDebug()<< QStringLiteral("OpenGL实现厂商的名字" )<< QString((char *)name);
    qDebug()<< QStringLiteral("渲染器标识符" )<< QString((char *)biaoshifu);
    qDebug()<< QStringLiteral("OpenGL实现的版本号：" )<< QString((char *)OpenGLVersion);
    qDebug()<< QStringLiteral("GLU工具库版本：" )<< QString((char *)gluVersion);
    Vertex vertexs[] = {Vertex(glm::vec3(-0.5,-0.5,0),glm::vec2(0,1.0)),
                        Vertex(glm::vec3(0,0.5,0),glm::vec2(0.5,0.0)),
                        Vertex(glm::vec3(0.5,-0.5,0),glm::vec2(1.0,1.0))};

    g_mesh =new Mesh(vertexs,sizeof(vertexs) / sizeof(vertexs[0]));
    g_shader = new Shader(":/shader/basicShader.vs",":/shader/basicShader.fs");

    g_texture = new Texture(":/shader/image.jpg");
//    g_texture = new Texture("/Users/miaozw/Pictures/222.jpeg");


    glutDisplayFunc(&myDisplay);
    //    initData();
    //    glutTimerFunc(2000, timerFunction, 1);
    glutMainLoop();
    return 0;
}
