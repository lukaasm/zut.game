/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_RENDERDEVICE
#define H_RENDERDEVICE

#include <GL/glew.h>
#include <GL/glfw.h>

#include "Common.h"
#include "VertexArrayObject.h"

namespace OGLHelper
{
    static void ActivateTexture(uint32 mode, uint32 textureId)
    {
        glActiveTexture(mode);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    static void DrawLines(VertexArrayObject& vao)
    {
        vao.Bind(ID_VAO);
        glDrawArrays(GL_LINES, 0, vao.ElementsCount());
        vao.Unbind(ID_VAO);
    }

    static void DrawTriangles(VertexArrayObject& vao)
    {
        vao.Bind(ID_VAO);
        glDrawArrays(GL_TRIANGLES, 0, vao.ElementsCount());
        vao.Unbind(ID_VAO);
    }

    static void DrawTriangleStrip(VertexArrayObject& vao)
    {
        vao.Bind(ID_VAO);
        glDrawArrays(GL_TRIANGLES, 0, vao.ElementsCount());
        vao.Unbind(ID_VAO);
    }
};

#endif
