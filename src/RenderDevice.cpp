/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "RenderDevice.h"

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"

RenderDevice::RenderDevice()
{
    camera = nullptr;
}

RenderDevice::~RenderDevice()
{
    delete_ptr(camera)
}

void RenderDevice::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderDevice::OnInit()
{
    camera = new Camera(this);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void RenderDevice::OnRender()
{
    GetCamera()->LookAt();
}

void RenderDevice::OnResize(int32 width, int32 height)
{
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);

    GetCamera()->OnResize();
}

void RenderDevice::OnUpdate(const uint32 diff)
{
    GetCamera()->OnUpdate(diff);
}

void RenderDevice::SetUniforms(Shader* shader, mat4 modelMatrix)
{
    glUniformMatrix4fv(shader->GetProjMatrixLocation(), 1, GL_FALSE, value_ptr(GetProjMatrix()));
    glUniformMatrix4fv(shader->GetViewMatrixLocation(), 1, GL_FALSE, value_ptr(GetViewMatrix()));
    glUniformMatrix4fv(shader->GetModelMatrixLocation(), 1, GL_FALSE, value_ptr(modelMatrix));
}

mat4 RenderDevice::GetProjMatrix() const
{
    return GetCamera()->GetProjMatrix();
}

mat4 RenderDevice::GetViewMatrix() const
{
    return GetCamera()->GetViewMatrix();
}

void RenderDevice::DrawLines(uint32 vao, uint32 start, uint32 size)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, start, size);
}
