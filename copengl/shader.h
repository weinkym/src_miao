#ifndef SHADER_H
#define SHADER_H
#include <OpenGL.h>
#include <QString>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

class Shader
{
public:
    Shader(const QString &filenName);
    ~Shader();
    void bind();

private:
    static const unsigned int NUM_SHADERS = 2;
    GLuint m_pragmram;
    GLuint m_shaders[];
};

#endif // SHADER_H
