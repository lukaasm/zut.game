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

    _moveFlags = MOVE_NONE;
}

void Camera::LookAt()
{
    _viewMatrix = lookAt(_position, _lookAt, _up);
}

void Camera::OnResize()
{
    _projMatrix = perspective(60.0f, float(_renderDevice->GetWidth() / _renderDevice->GetHeight()), 0.1f, 100.f);
}

void Camera::Move(MoveType type, float angleOrScale)
{
    switch (type)
    {
        case MOVE_FORWARD:
        {
            vec3 direction = normalize(_lookAt - _position) * angleOrScale;
            _position += direction;
            _lookAt += direction;
            break;
        }
        case MOVE_BACKWARD:
        {
            vec3 direction = normalize(_lookAt - _position) * angleOrScale;
            _position -= direction;
            _lookAt -= direction;
            break;
        }
        case MOVE_ROTATE_LEFT:
            RotateY(-angleOrScale);
            break;
        case MOVE_ROTATE_RIGHT:
            RotateY(angleOrScale);
            break;
        case MOVE_STRAFE_RIGHT:
        {
            vec3 direction = normalize(_right) * angleOrScale;
            _position += direction;
            _lookAt += direction;
            break;
        }
        case MOVE_STRAFE_LEFT:
        {
            vec3 direction = normalize(_right) * angleOrScale;
            _position -= direction;
            _lookAt -= direction;
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
    if (_moveFlags & MOVE_FORWARD)
        Move(MOVE_FORWARD, 0.2f);
    else if (_moveFlags & MOVE_BACKWARD)
        Move(MOVE_BACKWARD, 0.2f);

    if (_moveFlags & MOVE_STRAFE_LEFT)
        Move(MOVE_STRAFE_LEFT, 0.2f);
    else if (_moveFlags & MOVE_STRAFE_RIGHT)
        Move(MOVE_STRAFE_RIGHT, 0.2f);

    if (_moveFlags & MOVE_ROTATE_LEFT)
        Move(MOVE_ROTATE_LEFT, 3.5f);
    else if (_moveFlags & MOVE_ROTATE_RIGHT)
        Move(MOVE_ROTATE_RIGHT, 3.5f);
}

void Camera::AddMoveType(MoveType flag)
{
    if (flag & MOVE_FORWARD)
        _moveFlags = MoveType((_moveFlags & ~MOVE_BACKWARD) | flag);

    if (flag & MOVE_BACKWARD)
        _moveFlags = MoveType((_moveFlags & ~MOVE_FORWARD) | flag);

    if (flag & MOVE_ROTATE_LEFT)
        _moveFlags = MoveType((_moveFlags & ~MOVE_ROTATE_RIGHT) | flag);

    if (flag & MOVE_ROTATE_RIGHT)
        _moveFlags = MoveType((_moveFlags & ~MOVE_ROTATE_LEFT) | flag);

    if (flag & MOVE_STRAFE_LEFT)
        _moveFlags = MoveType((_moveFlags & ~MOVE_STRAFE_RIGHT) | flag);

    if (flag & MOVE_STRAFE_RIGHT)
        _moveFlags = MoveType((_moveFlags & ~MOVE_STRAFE_LEFT) | flag);
}

void Camera::ClearMoveType(MoveType flag)
{
    _moveFlags = MoveType(_moveFlags & ~flag);
}
