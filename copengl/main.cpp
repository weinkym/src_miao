#include <GL/glut.h>

#include <math.h>
#include <QString>
#include <QList>
#include "cutilinterface.h"
#include <QTimer>
#include <QDebug>

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

void myDisplay(void)
{
    int i;
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(3);
    glPointSize(3);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);  // Antialias the lines
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(0.5f, 0.0f, 0.5f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(g_rect.x(),g_rect.y());
    glVertex2f(g_rect.right(),g_rect.y());
    glVertex2f(g_rect.right(),g_rect.bottom());
    glVertex2f(g_rect.left(),g_rect.bottom());
    glVertex2f(g_rect.x(),g_rect.y());
    glEnd();

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
    qreal width = 0.28f;
    qreal height = 0.18f;
    for(auto obj:g_oldLineList)
    {
        QRectF rect1(obj.x1() - width / 2,obj.y1() - height / 2 ,width,height);
//        QRectF rect1(obj.x1() - width / 2,obj.y1() - height / 2 ,width,height);
        drawEllpsse(g_rect,rect1);
//        drawEllpsse(g_rect,rect2);
    }

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
    glutDisplayFunc(&myDisplay);
    initData();
//    glutTimerFunc(2000, timerFunction, 1);
    glutMainLoop();
    return 0;
}
