#ifdef Q_OS_WIN
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#include <math.h>
#include <QString>
#include <QList>
#include "cutilinterface.h"
#include <QTimer>
#include <QDebug>
#include "mesh.h"
#include "shader.h"

#include "texture.h"

const int n = 360;
const GLfloat R = 0.8f;
const GLfloat Pi = 3.1415926536f;

QRectF g_rect(-0.5,-0.5,1,1);
QList<QLineF> g_lineList;
QList<QLineF> g_oldLineList;
int g_count = 0;
qreal rand01()
{
    int base = 10000;
    return 2.0 * (qrand() % 10001) / 10000 - 1.0;
}

void initData()
{
    g_lineList.clear();
    g_oldLineList.clear();
    g_oldLineList.append(QLineF(0.0f,0.0f,0.0f,0.0f));
//    for(int i = 0; i < 1; ++i)
//    {
//        QLineF line(rand01(),rand01(),rand01(),rand01());
//        g_oldLineList.append(line);
//        if(CUtilInterface::adjuestLine(line,g_rect))
//        {
//            g_lineList.append(line);
//        }
//    }
}

void drawEllpsse(const QRectF& randRect,const QRectF &rect)
{
    QList<QPointF> points = CUtilInterface::getEllipsePoints(randRect,rect);
    qDebug()<<points.count();
    glColor3f(0.5f, 0.0f, 0.5f);
    glBegin(GL_POLYGON);
    for(auto obj:points)
    {
        glVertex2f(obj.x(),-obj.y());
    }
    glEnd();
}

Mesh *g_mesh = 0;
Shader *g_shader = 0;
Texture *g_texture = 0;
void myDisplay(void)
{
    int i;
    glClear(GL_COLOR_BUFFER_BIT);
//    glClearColor(1.0f,0.0f,0.0f,1.0f);
    glLineWidth(3);
    glPointSize(3);
//    glEnable(GL_BLEND);
//    glEnable(GL_LINE_SMOOTH);
//    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);  // Antialias the lines
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glColor3f(0.5f, 0.0f, 0.5f);

//    glColor3f(0.0f, 1.0f, 0.0f);
//    glBegin(GL_LINE_STRIP);
//    glVertex2f(g_rect.x(),g_rect.y());
//    glVertex2f(g_rect.right(),g_rect.y());
//    glVertex2f(g_rect.right(),g_rect.bottom());
//    glVertex2f(g_rect.left(),g_rect.bottom());
//    glVertex2f(g_rect.x(),g_rect.y());
//    glEnd();

//    if(g_count++ % 2 == 0)
//    {
//        glColor3f(0.5f, 0.0f, 0.5f);
//        glBegin(GL_LINES);
//        for(auto obj:g_lineList)
//        {
//            glVertex2f(obj.x1(),obj.y1());
//            glVertex2f(obj.x2(),obj.y2());
//        }
//        glEnd();
//    }
//    else
//    {
//        glColor4f(0.5f, 0.5f, 0.5f,0.5f);
//        glBegin(GL_LINES);
//        for(auto obj:g_oldLineList)
//        {
//            glVertex2f(obj.x1(),obj.y1());
//            glVertex2f(obj.x2(),obj.y2());
//        }
//        glEnd();
//    }

    glColor4f(0.5f, 0.5f, 0.5f,0.5f);
    qreal width = 0.5f;
    qreal height = 0.5f;
//    for(auto obj:g_oldLineList)
//    {
//        QRectF rect1(obj.x1() - width / 2,obj.y1() - height / 2 ,width,height);
//        QRectF rect1(obj.x1() - width / 2,obj.y1() - height / 2 ,width,height);
//        drawEllpsse(g_rect,rect1);
//        drawEllpsse(g_rect,rect2);
//    }

//    drawEllpsse(QRectF(-0.25,-0.25,0.5,0.5),QRectF(-0.25,-0.25,0.5,0.8));

    g_shader->bind();
    g_texture->Bind();
    g_mesh->draw();

//    glColor4f(0.5f, 0.5f, 0.5f,0.5f);
//    drawEllpsse(QRectF(-1,-1,2,2),QRectF(-0.5,-0.5,1,1));
    glFlush();
}

void timerFunction(int value)
{
    initData();
    glutPostRedisplay();
    glutTimerFunc(2000, timerFunction, 1);
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
    g_shader = new Shader("");

    g_texture = new Texture("");
    glutDisplayFunc(&myDisplay);
//    initData();
//    glutTimerFunc(2000, timerFunction, 1);
    glutMainLoop();
    return 0;
}
