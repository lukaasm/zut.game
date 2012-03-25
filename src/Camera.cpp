/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Camera.h"

#include <GL/glfw.h>
#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "RenderDevice.h"

using namespace glm;

Camera::Camera(RenderDevice* renderDevice)
{
    _renderDevice = renderDevice;

    _position = vec3(0.0f, 0.0f, -20.0f);
    _direction = vec3(0.0f, 0.0f, -1.0f);
    _right = vec3(1.0f, 0.0f, 0.0f);

    _up = cross(_right, _direction);
}

void Camera::LookAt()
{
    _viewMatrix = lookAt(GetPosition(), GetPosition()+GetDirection(), GetUpVector());
    _viewMatrix = translate(_viewMatrix, GetPosition());
}

void Camera::Move(MoveType type, float angleOrDist)
{
    switch (type)
    {
        case MOVE_FORWARD:
            _position += _direction*angleOrDist;
            break;
        case MOVE_BACKWARD:
            _position -= _direction*angleOrDist;
            break;
        case MOVE_ROTATE_LEFT:
        case MOVE_ROTATE_RIGHT:
            RotateY(angleOrDist);
            break;
        case MOVE_STRAFE_RIGHT:
            _position += _right*angleOrDist;
            break;
        case MOVE_STRAFE_LEFT:
            _position -= _right*angleOrDist;
            break;
    }
}

void Camera::RotateX(float angle)
{
    _direction = normalize(rotateX(_up, angle));
    _up = cross(_direction, _right);
}

void Camera::RotateY(float angle)
{
    _direction = normalize(rotateY(_right, angle));
    _right = cross(_direction, _up);
}

void Camera::OnResize()
{
    _projMatrix = perspective(60.0f, float(_renderDevice->GetWidth() / _renderDevice->GetHeight()), 0.1f, 100.f);
}

void Camera::OnUpdate(const uint32 diff)
{
}
