/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Camera.h"

#include <GL/glfw.h>
#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "MovableObject.h"
#include "RenderDevice.h"

Camera::Camera()
{
    //LookAt();
}

void Camera::OnUpdate(const uint32)
{
    LookAt();
}

void Camera::LookAt()
{
    // rotate own base position
    glm::vec3 pos = glm::vec3(0.0f, 1.5f, 3.0f);
    pos = glm::rotate(pos, owner->GetRotationX(), owner->up);
    pos = glm::rotate(pos, owner->GetRotationY(), owner->up);
    // move to owner position
    pos += owner->position;

    // and finally look at owner direction
    viewMatrix = glm::lookAt(pos, owner->position + owner->lookDirection, owner->up);
}

void Camera::OnResize(int32 width, int32 height)
{
    projMatrix = glm::perspective(60.0f, float(width/height), 0.1f, 100.0f);
    //frustum.Calculate(viewMatrix, projMatrix);
}
