/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Camera.h"

#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "DynamicObject.h"
#include "RenderDevice.h"

Camera::Camera()
{
}

void Camera::LookAt(const Position& pos, const glm::vec3& target, const glm::vec3& up)
{
    viewMatrix = glm::lookAt(pos, target, up);
}

void Camera::OnResize(int32 width, int32 height)
{
    projMatrix = glm::perspective(60.0f, float(width/height), 0.001f, 1000.0f);
}

void FppCamera::OnUpdate(const uint32& diff)
{
    if (DynamicObject* object = GetLinkedObject())
    {
        SetPosition(object->GetPosition());

        glm::vec3 lookPosition = position + object->GetDirVector();
        LookAt(position, lookPosition, object->GetUpVector());
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
    position.y += 1.4f; // redo it to use max.y model vertex pos
}

void TppCamera::OnUpdate(const uint32 & diff)
{
    if (DynamicObject* object = GetLinkedObject())
    {
        position = glm::vec3(0.0f, 1.5f, -3.5f);

        position = glm::rotate(position, object->GetOrientation(), object->GetUpVector());
        
        // move to owner position
        position += owner->GetPosition();

        glm::vec3 lookPosition = position + object->GetDirVector();
        LookAt(position, lookPosition, object->GetUpVector());
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

void EagleEyeCamera::OnUpdate(const uint32 &)
{
    if (DynamicObject* object = GetLinkedObject())
    {
        SetPosition(object->GetPosition());
        LookAt(position, object->GetPosition(), object->GetUpVector());
    }
}

void EagleEyeCamera::LinkTo(DynamicObject* o)
{
    owner = o;

    SetPosition(o->GetPosition());
}

void EagleEyeCamera::SetPosition(Position& pos)
{
    position = pos;
    position.z -= 0.005f;
    position.y += 10.0f; // redo it to use max.y model vertex pos
}
