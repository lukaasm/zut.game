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
    glm::vec3 pos = owner->position;
    pos.z += 5.0f;
    pos.y += 1.0f;

    viewMatrix = glm::lookAt(pos, owner->lookAt, owner->up);
}

void Camera::OnResize(int32 width, int32 height)
{
    projMatrix = glm::perspective(60.0f, float(width/height), 0.1f, 100.0f);
    //frustum.Calculate(viewMatrix, projMatrix);
}