#include "MovableObject.h"
#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

/// FIX ALL VECTOR CALCULATIONS ( UP,RIGHT etc ) !!!!!!!!!!
void Player::Move(MoveType type, float angleOrScale)
{
    switch (type)
    {
    case MOVE_FORWARD:
        {
            glm::vec3 offset = glm::normalize(lookAt - position) * angleOrScale;
            position += offset;
            lookAt += offset;
            break;
        }
    case MOVE_BACKWARD:
        {
            glm::vec3 offset = glm::normalize(lookAt - position) * angleOrScale;
            position -= offset;
            lookAt -= offset;
            break;
        }
    case MOVE_UPWARD:
        {
            glm::vec3 offset = glm::normalize(up) * angleOrScale;
            position += offset;
            lookAt += offset;
            break;
        }
    case MOVE_DOWNWARD:
        {
            glm::vec3 offset = glm::normalize(up) * angleOrScale;
            position -= offset;
            lookAt -= offset;
            break;
        }
    case MOVE_ROTATE_LEFT:
        {
            lookAt = glm::rotate(lookAt, angleOrScale, up);
            right = glm::cross(lookAt, up);
            break;
        }
    case MOVE_ROTATE_RIGHT:
        {
            lookAt = glm::rotate(lookAt, -angleOrScale, up);
            right = glm::cross(lookAt, up);
            break;
        }
    case MOVE_ROTATE_UP:
        {
            lookAt = glm::rotate(lookAt, angleOrScale, right);
            up = glm::cross(right, lookAt);
            break;
        }
    case MOVE_ROTATE_DOWN:
        {
            lookAt = glm::rotate(lookAt, -angleOrScale, right);
            up = glm::cross(right, lookAt);
            break;
        }
    case MOVE_STRAFE_RIGHT:
        {
            glm::vec3 offset = glm::normalize(right) * angleOrScale;
            position += offset;
            lookAt += offset;
            break;
        }
    case MOVE_STRAFE_LEFT:
        {
            glm::vec3 offset = glm::normalize(right) * angleOrScale;
            position -= offset;
            lookAt -= offset;
            break;
        }
    case MOVE_NONE:
    default:
        break;
    }

    if (position.y < (scale.y/2))
    {
       position.y = scale.y / 2;
       lookAt.y = position.y;
    }
}

void Player::OnUpdate(const uint32 diff)
{
    const float scale = 0.15f;

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
}

void Player::AddMoveType(MoveType flag)
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

void Player::ClearMoveType(MoveType flag)
{
    moveFlags = MoveType(moveFlags & ~flag);
}

Player::Player() : GameObject("cube.obj", "cube.tga")
{
    lookAt = glm::vec3(5.0f, 0.5f, -0.01f);

    moveFlags = MOVE_NONE;

    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
}
