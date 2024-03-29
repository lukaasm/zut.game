#include "VertexArrayObject.h"

#include <GL/glew.h>
#include <GL/glfw.h>

#include <algorithm>

void VertexArrayObject::CreateVertexArray()
{
    glGenVertexArrays(1, &id[ID_VAO]);
}

void VertexArrayObject::CreateVertexBuffer()
{
    glGenBuffers(1, &id[ID_VBO]);
}

void VertexArrayObject::Bind(uint8 i)
{
    if (i == ID_VAO)
    {
        glBindVertexArray(id[i]);
        std::for_each(attribs.begin(), attribs.end(), [this](uint32& attrib) { EnableAttrib(attrib); });
    }
    else if (i == ID_VBO)
        glBindBuffer(GL_ARRAY_BUFFER, id[i]);
}

void VertexArrayObject::Unbind(uint8 i)
{
    if (i == ID_VAO)
    {
        std::for_each(attribs.begin(), attribs.end(), [this](uint32& attrib) { DisableAttrib(attrib); });
        glBindVertexArray(0);
    }
    else if (i == ID_VBO)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArrayObject::FillBuffer(uint32 drawMode, void* data, uint32 size)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, drawMode);
}

void VertexArrayObject::AddAttribToBuffer(uint32 att, uint8 elements, uint32 elType, bool normalized, uint32 size, void* offset)
{
    attribs.push_back(att);
    glVertexAttribPointer(att, elements, elType, normalized, size, offset);
}

void VertexArrayObject::EnableAttrib(uint32 att)
{
    glEnableVertexAttribArray(att);
}

void VertexArrayObject::DisableAttrib(uint32 att)
{
    glDisableVertexAttribArray(att);
}

VertexArrayObject::~VertexArrayObject()
{
    if (!releaseGPUResources)
        return;

    glDeleteVertexArrays(1, &id[ID_VAO]);
    glDeleteBuffers(1, &(id[ID_VBO]));
}
