/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "RenderDevice.h"

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

RenderDevice::RenderDevice()
{
}

void RenderDevice::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderDevice::OnInit()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void RenderDevice::OnRender()
{
    _viewMatrix = translate(mat4(1.0f), vec3(-2.0f, -2.0f, -3.0f));
    _modelMatrix = scale(glm::mat4(1.0f), vec3(0.1f));

}

void RenderDevice::OnResize(int32 width, int32 height)
{
    _width = width;
    _height = height;

    glViewport(0, 0, width, height);

    _projMatrix = perspective(60.0f, float(_width / _height), 0.1f, 100.f);
}

void RenderDevice::OnUpdate(const uint32 diff)
{
}

void RenderDevice::SetUniforms(Shader* shader)
{
    glUniformMatrix4fv(shader->GetProjMatrixLocation(), 1, GL_FALSE, value_ptr(GetProjMatrix()));
    glUniformMatrix4fv(shader->GetViewMatrixLocation(), 1, GL_FALSE, value_ptr(GetViewMatrix()));
    glUniformMatrix4fv(shader->GetModelMatrixLocation(), 1, GL_FALSE, value_ptr(GetModelMatrix()));
}
