#ifndef MESH_H
#define MESH_H
#include <OpenGL.h>
#include "glm/glm.hpp"

class Vertex
{
public:
    Vertex(const glm::vec3 &pos,const glm::vec2& texCoord);
    glm::vec3 GetPos();
    glm::vec2 GetTextureCoord();

private:
    glm::vec3 pos;
    glm::vec2 texCoord;
};

class Mesh
{
public:
    Mesh(Vertex *vertex,unsigned int size );
    ~Mesh();
    void draw();

private:
    enum
    {
        POSTION_VB,
        TexCOORD_VB,
        NUM_BUFFERS
    };
    GLuint m_vertextArrayObject;
    GLuint m_vertextArrayBuffer[NUM_BUFFERS];
    unsigned int m_drawCount;
};

#endif // MESH_H
