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

void RenderDevice::OnResize(int32 width, int32 height)
{
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);
}

void RenderDevice::OnUpdate(const uint32 diff)
{
}

void RenderDevice::SetUniforms(Shader* shader, const glm::mat4& projMatrix, const glm::mat4& viewMatrix, const glm::mat4& modelMatrix, float hasTexture)
{
    glm::mat4 mvpMatrix = projMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv(shader->GetMVPMatrixLocation(), 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    glUniform1i(shader->textLoc, 0);
    glUniform1f(shader->textEnabledLoc, hasTexture);
}

// For Text2d render only
void RenderDevice::SetUniforms(Shader* shader)
{
    glUniform1i(shader->textLoc, 0);
    glUniform1f(shader->textEnabledLoc, float(1.0f));
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

void RenderDevice::OnRenderStart()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderDevice::OnRenderEnd()
{
    glDisable(GL_DEPTH_TEST);
}

void RenderDevice::OnInit()
{

}
