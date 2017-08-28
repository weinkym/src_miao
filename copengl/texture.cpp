#include "texture.h"
#include <QImage>
#include <QDebug>

Texture::Texture(const QString &fileName)
{
//    QImage image("/Users/miaozw/Documents/111.jpg");
    QImage image("/Users/miaozw/Documents/222.jpeg");


    uchar* data = image.bits();

    qDebug()<<image.size();


    glGenTextures(1,&m_texture);
    glBindTexture(GL_TEXTURE_2D,m_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image.width(),image.height(),
                 0,GL_RGBA,GL_UNSIGNED_BYTE,data);
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D,m_texture);
}

Texture::~Texture()
{
    glDeleteTextures(1,&m_texture);
}
