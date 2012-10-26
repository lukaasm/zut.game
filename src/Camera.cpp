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

Camera::Camera()
{
    position = vec3(5.0f, 0.5f, 0.0f);
    lookAt = vec3(5.0f, 0.5f, -5.0f);

    up = vec3(0.0f, 1.0f, 0.0f);
    right = vec3(1.0f, 0.0f, 0.0f);

    moveFlags = MOVE_NONE;
}

void Camera::LookAt()
{
    viewMatrix = glm::lookAt(position, lookAt, up);
}

void Camera::OnResize(int32 width, int32 height)
{
    projMatrix = perspective(60.0f, float(width/height), 0.1f, 100.f);
}

void Camera::Move(MoveType type, float angleOrScale)
{
    switch (type)
    {
        case MOVE_FORWARD:
        {
            vec3 offset = normalize(lookAt - position) * angleOrScale;
            position += offset;
            lookAt += offset;
            break;
        }
        case MOVE_BACKWARD:
        {
            vec3 offset = normalize(lookAt - position) * angleOrScale;
            position -= offset;
            lookAt -= offset;
            break;
        }
        case MOVE_UPWARD:
        {
            vec3 offset = normalize(up) * angleOrScale;
            position += offset;
            lookAt += offset;
            break;
        }
        case MOVE_DOWNWARD:
        {
            vec3 offset = normalize(up) * angleOrScale;
            position -= offset;
            lookAt -= offset;
            break;
        }
        case MOVE_ROTATE_LEFT:
        {
            lookAt = rotate(lookAt, angleOrScale, up);
            right = cross(lookAt, up);
            break;
        }
        case MOVE_ROTATE_RIGHT:
        {
            lookAt = rotate(lookAt, -angleOrScale, up);
            right = cross(lookAt, up);
            break;
        }
        case MOVE_ROTATE_UP:
        {
            lookAt = rotate(lookAt, angleOrScale, right);
            up = cross(right, lookAt);
            break;
        }
        case MOVE_ROTATE_DOWN:
        {
            lookAt = rotate(lookAt, -angleOrScale, right);
            up = cross(right, lookAt);
            break;
        }
        case MOVE_STRAFE_RIGHT:
        {
            vec3 offset = normalize(right) * angleOrScale;
            position += offset;
            lookAt += offset;
            break;
        }
        case MOVE_STRAFE_LEFT:
        {
            vec3 offset = normalize(right) * angleOrScale;
            position -= offset;
            lookAt -= offset;
            break;
        }
    }
}

void Camera::OnUpdate(const uint32 diff)
{
    const float scale = 0.2f;

    if (moveFlags & MOVE_FORWARD)
        Move(MOVE_FORWARD, scale);
    else if (moveFlags & MOVE_BACKWARD)
        Move(MOVE_BACKWARD, scale);

    if (moveFlags & MOVE_UPWARD)
        Move(MOVE_UPWARD, scale);
    else if (moveFlags & MOVE_DOWNWARD)
        Move(MOVE_DOWNWARD, scale);

    if (moveFlags & MOVE_STRAFE_LEFT)
        Move(MOVE_STRAFE_LEFT, scale);
    else if (moveFlags & MOVE_STRAFE_RIGHT)
        Move(MOVE_STRAFE_RIGHT, scale);

    if (moveFlags & MOVE_ROTATE_LEFT)
        Move(MOVE_ROTATE_LEFT, 3.5f);
    else if (moveFlags & MOVE_ROTATE_RIGHT)
        Move(MOVE_ROTATE_RIGHT, 3.5f);

    if (moveFlags & MOVE_ROTATE_UP)
        Move(MOVE_ROTATE_UP, 3.5f);
    else if (moveFlags & MOVE_ROTATE_DOWN)
        Move(MOVE_ROTATE_DOWN, 3.5f);

    LookAt();
}

void Camera::AddMoveType(MoveType flag)
{
    if (flag & MOVE_FORWARD)
        moveFlags = MoveType((moveFlags & ~MOVE_BACKWARD) | flag);

    if (flag & MOVE_BACKWARD)
        moveFlags = MoveType((moveFlags & ~MOVE_FORWARD) | flag);

    if (flag & MOVE_UPWARD)
        moveFlags = MoveType((moveFlags & ~MOVE_DOWNWARD) | flag);

    if (flag & MOVE_DOWNWARD)
        moveFlags = MoveType((moveFlags & ~MOVE_UPWARD) | flag);

    if (flag & MOVE_ROTATE_LEFT)
        moveFlags = MoveType((moveFlags & ~MOVE_ROTATE_RIGHT) | flag);

    if (flag & MOVE_ROTATE_RIGHT)
        moveFlags = MoveType((moveFlags & ~MOVE_ROTATE_LEFT) | flag);

    if (flag & MOVE_ROTATE_UP)
        moveFlags = MoveType((moveFlags & ~MOVE_ROTATE_DOWN) | flag);

    if (flag & MOVE_ROTATE_DOWN)
        moveFlags = MoveType((moveFlags & ~MOVE_ROTATE_UP) | flag);

    if (flag & MOVE_STRAFE_LEFT)
        moveFlags = MoveType((moveFlags & ~MOVE_STRAFE_RIGHT) | flag);

    if (flag & MOVE_STRAFE_RIGHT)
        moveFlags = MoveType((moveFlags & ~MOVE_STRAFE_LEFT) | flag);
}

void Camera::ClearMoveType(MoveType flag)
{
    moveFlags = MoveType(moveFlags & ~flag);
}
