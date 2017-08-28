#include "shader.h"
#include <QFile>
#include <QDebug>
#include <stdio.h>
#include <iostream>

static void CheckShaderError(GLuint shader,GLuint flag,bool isProgram,const std::string &errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = {0};
    if(isProgram)
    {
        glGetProgramiv(shader,flag,&success);
    }
    else
    {
        glGetShaderiv(shader,flag,&success);
    }
    if(success == GL_FALSE)
    {
        if(isProgram)
        {
            glGetProgramInfoLog(shader,sizeof(error),NULL,error);
        }
        else
        {
            glGetShaderInfoLog(shader,sizeof(error),NULL,error);
        }
        std::cerr <<errorMessage << error<< std::endl;

    }
}

static std::string LoaderShader(const QString &fileName)
{
    QString output;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
        QString line = file.readLine();
        output.append(line + "\n");
        }
    }
//    qDebug()<<"output.toStdString()"<<output.toStdString().c_str();
    return output.toStdString();
}

static GLuint CreateShader(const std::string &text,GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    if(shader == 0)
    {
        qDebug()<<"TTTTV";
        std::cerr << "error shader "<<std::endl;
    }
    const GLchar* shaderSourceString[1];
    GLint shaderLength[1];
    shaderSourceString[0]=text.c_str();
    shaderLength[0] = text.length();
    glShaderSource(shader,1,shaderSourceString,shaderLength);

    glCompileShader(shader);

    CheckShaderError(shader,GL_COMPILE_STATUS,false,"glCompileShader");
    return shader;
}

Shader::Shader(const QString &filenName)
{
    m_pragmram = glCreateProgram();

    m_shaders[0] = CreateShader(LoaderShader("/Users/miaozw/Documents/basicShader.vs"),GL_VERTEX_SHADER);
    m_shaders[1] = CreateShader(LoaderShader("/Users/miaozw/Documents/basicShader.fs"),GL_FRAGMENT_SHADER);

    for(unsigned int i = 0; i < NUM_SHADERS;i++)
    {
        glAttachShader(m_pragmram,m_shaders[i]);
    }

    glBindAttribLocation(m_pragmram,0,"position");
    glBindAttribLocation(m_pragmram,1,"texCoord");

    glLinkProgram(m_pragmram);
    CheckShaderError(m_pragmram,GL_LINK_STATUS,true,"program link is error");
    glValidateProgram(m_pragmram);
    CheckShaderError(m_pragmram,GL_VALIDATE_STATUS,true,"program GL_VALIDATE_STATUS is error");
}

Shader::~Shader()
{
    for(unsigned int i = 0; i < NUM_SHADERS;i++)
    {
        glDetachShader(m_pragmram,m_shaders[i]);
    }
    glDeleteProgram(m_pragmram);
}

void Shader::bind()
{
    glUseProgram(m_pragmram);
}
