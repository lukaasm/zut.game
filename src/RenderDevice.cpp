/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "RenderDevice.h"

#include <GL/glew.h>
#include <GL/glfw.h>
#include <iostream>

#include "Camera.h"
#include "Exception.h"
#include "SceneMgr.h"
#include "Shader.h"
#include "VertexArrayObject.h"

RenderDevice::RenderDevice()
{

}

RenderDevice::~RenderDevice()
{

}

void RenderDevice::OnResize(int32 width, int32 height)
{
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);
}

void RenderDevice::OnUpdate(const uint32 & diff)
{

}

void RenderDevice::DrawLines(VertexArrayObject& vao)
{
    vao.Bind(ID_VAO);
    glDrawArrays(GL_LINES, 0, vao.ElementsCount());
    vao.Unbind(ID_VAO);
}

void RenderDevice::DrawTriangles(VertexArrayObject& vao)
{
    vao.Bind(ID_VAO);
    glDrawArrays(GL_TRIANGLES, 0, vao.ElementsCount());
    vao.Unbind(ID_VAO);
}

void RenderDevice::ActivateTexture(uint32 mode, uint32 textureId)
{
    if (textureId == 0)
        return;

    glActiveTexture(mode);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void RenderDevice::OnRenderStart()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderDevice::OnRenderEnd()
{
    glDisable(GL_DEPTH_TEST);
}

void RenderDevice::OnInit()
{

}
