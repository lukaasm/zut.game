/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Camera.h"

#include <GL/glfw.h>
#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "DynamicObject.h"
#include "RenderDevice.h"

Camera::Camera()
{
}

void Camera::LookAt(Position& pos, glm::vec3& target, glm::vec3& up)
{
    viewMatrix = glm::lookAt(pos, target, up);
}

void Camera::OnResize(int32 width, int32 height)
{
    projMatrix = glm::perspective(60.0f, float(width/height), 0.1f, 100.0f);
}

void FppCamera::OnUpdate(const uint32 & diff)
{
    if (DynamicObject* object = GetLinkedObject())
    {
        glm::vec3 offset = object->GetPosition() - position;

        SetPosition(object->GetPosition());

        glm::vec3 lookPosition = position + object->lookDirection;
        LookAt(position, lookPosition, object->up);
    }
}

void FppCamera::LinkTo(DynamicObject* o)
{
    owner = o;

    SetPosition(o->GetPosition());
}

void FppCamera::SetPosition(Position& pos)
{
    position = pos;
    position.y += 1.0f; // redo it to use max.y model vertex pos
}

void TppCamera::OnUpdate(const uint32 & diff)
{
    if (DynamicObject* object = GetLinkedObject())
    {
        glm::vec3 offset = object->GetPosition() - position;

        position = glm::vec3(0.0f, 1.5f, 3.5f);

        position = glm::rotate(position, object->GetRotationX(), glm::cross(object->up, object->lookDirection));
        position = glm::rotate(position, object->GetRotationY(), object->up);
        // move to owner position
        position += owner->GetPosition();

        glm::vec3 lookPosition = position + object->lookDirection;
        LookAt(position, lookPosition, object->up);
    }
}

void TppCamera::LinkTo(DynamicObject* o)
{
    owner = o;
}

void TppCamera::SetPosition(Position& pos)
{
    position = pos;
}
