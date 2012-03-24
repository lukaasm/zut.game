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
    _camera = NULL;

    _modelMatrix = mat4(1.0f);
}

RenderDevice::~RenderDevice()
{
    delete_ptr(_camera)
}

void RenderDevice::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderDevice::OnInit()
{
    _camera = new Camera(this);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void RenderDevice::OnRender()
{
    _viewMatrix = translate(mat4(1.0f), vec3(-10.0f, -10.0f, 3.0f));
}

void RenderDevice::OnResize(int32 width, int32 height)
{
    _width = width;
    _height = height;

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
    glUniformMatrix4fv(shader->GetModelMatrixLocation(), 1, GL_FALSE, value_ptr(GetModelMatrix()*modelMatrix));
}

mat4 RenderDevice::GetProjMatrix() const
{
    return GetCamera()->GetProjMatrix();
}
