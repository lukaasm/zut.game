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
            vec3 offset = normalize(_lookAt - _position) * angleOrScale;
            _position += offset;
            _lookAt += offset;
            break;
        }
        case MOVE_BACKWARD:
        {
            vec3 offset = normalize(_lookAt - _position) * angleOrScale;
            _position -= offset;
            _lookAt -= offset;
            break;
        }
        case MOVE_UPWARD:
        {
            vec3 offset = normalize(_up) * angleOrScale;
            _position += offset;
            _lookAt += offset;
            break;
        }
        case MOVE_DOWNWARD:
        {
            vec3 offset = normalize(_up) * angleOrScale;
            _position -= offset;
            _lookAt -= offset;
            break;
        }
        case MOVE_ROTATE_LEFT:
        {
            _lookAt = rotate(_lookAt, angleOrScale, _up);
            _right = cross(_lookAt, _up);
            break;
        }
        case MOVE_ROTATE_RIGHT:
        {
            _lookAt = rotate(_lookAt, -angleOrScale, _up);
            _right = cross(_lookAt, _up);
            break;
        }
        case MOVE_ROTATE_UP:
        {
            _lookAt = rotate(_lookAt, angleOrScale, _right);
            _up = cross(_right, _lookAt);
            break;
        }
        case MOVE_ROTATE_DOWN:
        {
            _lookAt = rotate(_lookAt, -angleOrScale, _right);
            _up = cross(_right, _lookAt);
            break;
        }
        case MOVE_STRAFE_RIGHT:
        {
            vec3 offset = normalize(_right) * angleOrScale;
            _position += offset;
            _lookAt += offset;
            break;
        }
        case MOVE_STRAFE_LEFT:
        {
            vec3 offset = normalize(_right) * angleOrScale;
            _position -= offset;
            _lookAt -= offset;
            break;
        }
    }
}

void Camera::OnUpdate(const uint32 diff)
{
    const float scale = 0.2f;

    if (_moveFlags & MOVE_FORWARD)
        Move(MOVE_FORWARD, scale);
    else if (_moveFlags & MOVE_BACKWARD)
        Move(MOVE_BACKWARD, scale);

    if (_moveFlags & MOVE_UPWARD)
        Move(MOVE_UPWARD, scale);
    else if (_moveFlags & MOVE_DOWNWARD)
        Move(MOVE_DOWNWARD, scale);

    if (_moveFlags & MOVE_STRAFE_LEFT)
        Move(MOVE_STRAFE_LEFT, scale);
    else if (_moveFlags & MOVE_STRAFE_RIGHT)
        Move(MOVE_STRAFE_RIGHT, scale);

    if (_moveFlags & MOVE_ROTATE_LEFT)
        Move(MOVE_ROTATE_LEFT, 3.5f);
    else if (_moveFlags & MOVE_ROTATE_RIGHT)
        Move(MOVE_ROTATE_RIGHT, 3.5f);

    if (_moveFlags & MOVE_ROTATE_UP)
        Move(MOVE_ROTATE_UP, 3.5f);
    else if (_moveFlags & MOVE_ROTATE_DOWN)
        Move(MOVE_ROTATE_DOWN, 3.5f);
}

void Camera::AddMoveType(MoveType flag)
{
    if (flag & MOVE_FORWARD)
        _moveFlags = MoveType((_moveFlags & ~MOVE_BACKWARD) | flag);

    if (flag & MOVE_BACKWARD)
        _moveFlags = MoveType((_moveFlags & ~MOVE_FORWARD) | flag);

    if (flag & MOVE_UPWARD)
        _moveFlags = MoveType((_moveFlags & ~MOVE_DOWNWARD) | flag);

    if (flag & MOVE_DOWNWARD)
        _moveFlags = MoveType((_moveFlags & ~MOVE_UPWARD) | flag);

    if (flag & MOVE_ROTATE_LEFT)
        _moveFlags = MoveType((_moveFlags & ~MOVE_ROTATE_RIGHT) | flag);

    if (flag & MOVE_ROTATE_RIGHT)
        _moveFlags = MoveType((_moveFlags & ~MOVE_ROTATE_LEFT) | flag);

    if (flag & MOVE_ROTATE_UP)
        _moveFlags = MoveType((_moveFlags & ~MOVE_ROTATE_DOWN) | flag);

    if (flag & MOVE_ROTATE_DOWN)
        _moveFlags = MoveType((_moveFlags & ~MOVE_ROTATE_UP) | flag);

    if (flag & MOVE_STRAFE_LEFT)
        _moveFlags = MoveType((_moveFlags & ~MOVE_STRAFE_RIGHT) | flag);

    if (flag & MOVE_STRAFE_RIGHT)
        _moveFlags = MoveType((_moveFlags & ~MOVE_STRAFE_LEFT) | flag);
}

void Camera::ClearMoveType(MoveType flag)
{
    _moveFlags = MoveType(_moveFlags & ~flag);
}
