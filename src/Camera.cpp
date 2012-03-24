/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderDevice.h"

Camera::Camera(RenderDevice* renderDevice)
{
    _renderDevice = renderDevice;

    _position = vec3(0.0f, 0.0f, 0.0f);
    _orientation = vec3(0.0f, 0.0f, 0.0f);
    _forward = vec3(0.0f, 0.0f, 0.0f);

    _lookAtMatrix = mat4(1.0f);
}

void Camera::OnResize()
{
    _projMatrix = perspective(60.0f, float(_renderDevice->GetWidth() / _renderDevice->GetHeight()), 0.1f, 100.f);
}

void Camera::OnUpdate(const uint32 diff)
{

}
