#include "MovableObject.h"

#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Input.h"

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
    if (IsControllable())
    {
        for (Keyboard::KeysMap::const_iterator i = sKeyboard->GetKeysMap().begin(); i != sKeyboard->GetKeysMap().end(); ++i)
        {
            const MoveFlag& flag = Keyboard::Key2MoveFlag(i->first);
            if (flag.apply == MOVE_NONE)
                continue;

            if (i->second)
                AddMoveType(flag);
            else
                ClearMoveType(flag.apply);
        }
    }

    for (uint8 i = 0; i < MAX_MOVE_FLAGS; ++i)
    {
        const MoveFlag& flag = moveflags[i];
        if (moveFlags & flag.apply)
            Move(flag.apply, flag.speed);
    }
}

void Player::AddMoveType(MoveFlag flag)
{
    moveFlags = MoveType((moveFlags | flag.apply) & ~flag.remove);
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
