#include "mesh.h"
#include <OpenGL/gl.h>
#include <vector>

Vertex::Vertex(const glm::vec3 &pos, const glm::vec2 &texCoord)
{
    this->pos = pos;
    this->texCoord = texCoord;
}

glm::vec3 Vertex::GetPos()
{
    return pos;
}

glm::vec2 Vertex::GetTextureCoord()
{
    return texCoord;

}
//==============
Mesh::Mesh(Vertex *vertex, unsigned int size)
{
    m_drawCount = size;
    glGenVertexArraysAPPLE(1,&m_vertextArrayObject);
    glBindVertexArrayAPPLE(m_vertextArrayObject);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texcoords;
    positions.reserve(size);
    texcoords.reserve(size);
    for(unsigned int i = 0; i < size; i++)
    {
        positions.push_back(vertex[i].GetPos());
        texcoords.push_back(vertex[i].GetTextureCoord());
    }

    glGenBuffers(NUM_BUFFERS,m_vertextArrayBuffer);

    glBindBuffer(GL_ARRAY_BUFFER,m_vertextArrayBuffer[POSTION_VB]);
    glBufferData(GL_ARRAY_BUFFER,m_drawCount * sizeof(positions[0]),&positions[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m_vertextArrayBuffer[TexCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER,m_drawCount * sizeof(texcoords[0]),&texcoords[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);

    glBindVertexArrayAPPLE(0);

}

Mesh::~Mesh()
{
    glDeleteVertexArraysAPPLE(1,&m_vertextArrayObject);
}

void Mesh::draw()
{
    glBindVertexArrayAPPLE(m_vertextArrayObject);
    glDrawArrays(GL_TRIANGLES,0,3);
//    glDrawArrays(GL_POINTS,0,3);
    glBindVertexArrayAPPLE(0);
}


