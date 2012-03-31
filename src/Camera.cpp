/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Camera.h"

#include <GL/glfw.h>
#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "RenderDevice.h"

using namespace glm;

Camera::Camera(RenderDevice* renderDevice)
{
    _renderDevice = renderDevice;

    _position = vec3(2.0f, 0.5f, 0.0f);
    _lookAt = vec3(2.0f, 0.5f, -5.0f);

    _up = vec3(0.0f, 1.0f, 0.0f);
    _right = vec3(1.0f, 0.0f, 0.0f);
}

void Camera::LookAt()
{
    _viewMatrix = lookAt(_position, _lookAt, _up);
}

void Camera::OnResize()
{
    _projMatrix = perspective(60.0f, float(_renderDevice->GetWidth() / _renderDevice->GetHeight()), 0.1f, 100.f);
}

void Camera::Move(MoveType type, float angleOrDist)
{
    switch (type)
    {
        case MOVE_FORWARD:
        {
            vec3 direction = normalize(_lookAt - _position);
            _position += direction;
            _lookAt += direction;
            break;
        }
        case MOVE_BACKWARD:
        {
            vec3 direction = normalize(_lookAt - _position);
            _position -= direction;
            _lookAt -= direction;
            break;
        }
        case MOVE_ROTATE_LEFT:
            RotateY(3.5f);
            break;
        case MOVE_ROTATE_RIGHT:
            RotateY(-3.5f);
            break;
        case MOVE_STRAFE_RIGHT:
        {
            _position += normalize(_right);
            _lookAt += normalize(_right);
            break;
        }
        case MOVE_STRAFE_LEFT:
        {
            _position -= normalize(_right);
            _lookAt -= normalize(_right);
            break;
        }
    }
}

void Camera::RotateX(float angle)
{
    //_direction = rotateX(_up, angle));
    //_up = cross(_direction, _right);
}

void Camera::RotateY(float angle)
{
    _lookAt = rotate(_lookAt, angle, _up);
    _right = cross(_lookAt, _up);
}

void Camera::OnUpdate(const uint32 diff)
{
}
