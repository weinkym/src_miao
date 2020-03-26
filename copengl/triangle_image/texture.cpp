#include "texture.h"
#include <QImage>
#include <QDebug>

Texture::Texture(const QString &fileName)
{
    QImage image(fileName);
    image.convertToFormat(QImage::Format_RGB16);
    uchar* data = image.bits();
    qDebug()<<image.format();

    for(int index = 0; index < image.byteCount() - 4; )
    {
//        data[index] /= 2;
//        data[index+1] /= 2;
//        data[index+2] /= 2;

//        data[index] = 0;
//        data[index+1] = 0;
//        data[index+2] = 0;
        index += 4;
    }



//    qDebug()<<"len="<<len;

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
