/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "RenderDevice.h"

#include <GL/glew.h>
#include <GL/glfw.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "SceneMgr.h"
#include "Shader.h"

RenderDevice::RenderDevice()
{
}

RenderDevice::~RenderDevice()
{
}

void RenderDevice::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderDevice::OnInit()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void RenderDevice::OnRender()
{
}

void RenderDevice::OnResize(int32 width, int32 height)
{
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);
}

void RenderDevice::OnUpdate(const uint32 diff)
{
}

void RenderDevice::SetUniforms(Shader* shader, mat4& modelMatrix, mat4& projMatrix, mat4& viewMatrix, float hasTexture)
{
    glUniformMatrix4fv(shader->GetProjMatrixLocation(), 1, GL_FALSE, value_ptr(projMatrix));
    glUniformMatrix4fv(shader->GetViewMatrixLocation(), 1, GL_FALSE, value_ptr(viewMatrix));
    glUniformMatrix4fv(shader->GetModelMatrixLocation(), 1, GL_FALSE, value_ptr(modelMatrix));

    glUniform1i(shader->textLoc, 0);
    glUniform1f(shader->textEnabledLoc, hasTexture);
}

void RenderDevice::DrawLines(uint32 vao, uint32 start, uint32 size)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, start, size);
}

void RenderDevice::DrawTriangles(uint32 vao, uint32 start, uint32 size)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, start, size);
}

void RenderDevice::ActivateTexture(uint32 mode, uint32 textureId)
{
    if (textureId == 0)
        return;

    glActiveTexture(mode);
    glBindTexture(GL_TEXTURE_2D, textureId);
}
