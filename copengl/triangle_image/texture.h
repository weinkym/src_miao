#ifndef TEXTURE_H
#define TEXTURE_H
#include <QString>

#include <OPENGL/gl.h>
class Texture
{
public:
    Texture(const QString &fileName);
    void Bind();
    ~Texture();

private:
    GLuint m_texture;
};

#endif // TEXTURE_H
