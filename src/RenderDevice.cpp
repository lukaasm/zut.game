/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "RenderDevice.h"

#include <GL/glew.h>
#include <GL/glfw.h>

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

void RenderDevice::SetUniforms(Shader* shader, const glm::mat4& projMatrix, const glm::mat4& viewMatrix, const glm::mat4& modelMatrix, float hasTexture)
{
    try
    {
        glm::mat4 MV = viewMatrix * modelMatrix;
        glm::mat4 MVP = projMatrix * MV;

        glm::mat4 N = glm::transpose(glm::inverse(MV));
       // glm::mat3 X = glm::inverseTranspose(glm::mat3(MV));

        glUniformMatrix4fv(shader->GetUniformLocation("in_MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
        glUniformMatrix4fv(shader->GetUniformLocation("in_MV"), 1, GL_FALSE, glm::value_ptr(MV));
        glUniformMatrix4fv(shader->GetUniformLocation("in_N"), 1, GL_FALSE, glm::value_ptr(N));
        glUniformMatrix3fv(shader->GetUniformLocation("in_V"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

        glUniform1i(shader->GetUniformLocation("textureSampler"), 0);
        glUniform1f(shader->GetUniformLocation("textureFlag"), hasTexture);
    }
    catch (Exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

// For Text2d render only
void RenderDevice::SetUniforms(Shader* shader)
{
    glUniform1f(shader->GetUniformLocation("textureSampler"), 0);
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
