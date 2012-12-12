/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "RenderDevice.h"



void OGLHelper::DrawLines(VertexArrayObject& vao)
{
    vao.Bind(ID_VAO);
    glDrawArrays(GL_LINES, 0, vao.ElementsCount());
    vao.Unbind(ID_VAO);
}

void OGLHelper::DrawTriangles(VertexArrayObject& vao)
{
    vao.Bind(ID_VAO);
    glDrawArrays(GL_TRIANGLES, 0, vao.ElementsCount());
    vao.Unbind(ID_VAO);
}

void OGLHelper::ActivateTexture(uint32 mode, uint32 textureId)
{
    glActiveTexture(mode);
    glBindTexture(GL_TEXTURE_2D, textureId);
}
