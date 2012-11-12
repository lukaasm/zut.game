#include "MovableObject.h"

#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Input.h"

/// FIX ALL VECTOR CALCULATIONS ( UP,RIGHT etc ) !!!!!!!!!!
void Player::Move(MoveFlags flags, float angleOrScale)
{
    if (flags & MOVE_FLAG_FORWARD)
    {
        glm::vec3 offset = glm::normalize(lookAt - position) * angleOrScale;
        position += offset;
        lookAt += offset;
    }

    if (flags & MOVE_FLAG_BACKWARD)
    {
        glm::vec3 offset = glm::normalize(lookAt - position) * angleOrScale;
        position -= offset;
        lookAt -= offset;
    }

    if (flags & MOVE_FLAG_UPWARD)
    {
        glm::vec3 offset = glm::normalize(up) * angleOrScale;
        position += offset;
        lookAt += offset;
    }

    if (flags & MOVE_FLAG_DOWNWARD)
    {
        glm::vec3 offset = glm::normalize(up) * angleOrScale;
        position -= offset;
        lookAt -= offset;
    }

    if (flags & MOVE_FLAG_ROTATE_LEFT)
    {
        lookAt = glm::rotate(lookAt, angleOrScale, up);
        right = glm::cross(lookAt, up);
    }

    if (flags & MOVE_FLAG_ROTATE_RIGHT)
    {
        lookAt = glm::rotate(lookAt, -angleOrScale, up);
        right = glm::cross(lookAt, up);
    }

    if (flags & MOVE_FLAG_STRAFE_RIGHT)
    {
        glm::vec3 offset = glm::normalize(right) * angleOrScale;
        position += offset;
        lookAt += offset;
    }

    if (flags & MOVE_FLAG_STRAFE_LEFT)
    {
        glm::vec3 offset = glm::normalize(right) * angleOrScale;
        position -= offset;
        lookAt -= offset;
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
            const MoveInfo& info = Keyboard::Key2MoveInfo(i->first);
            if (info.apply == MOVE_FLAG_NONE)
                continue;

            if (i->second)
                AddMoveType(info);
            else
                ClearMoveType(info.apply);
        }
    }

    for (uint8 i = 0; i < MAX_MOVE_TYPES; ++i)
    {
        const MoveInfo& info = moveInfos[i];
        if (moveFlags & info.apply)
            Move(info.apply, info.speed);
    }
}

void Player::AddMoveType(MoveInfo flag)
{
    moveFlags = MoveFlags((moveFlags | flag.apply) & ~flag.remove);
}

void Player::ClearMoveType(MoveFlags flag)
{
    moveFlags = MoveFlags(moveFlags & ~flag);
}

Player::Player() : GameObject("cube.obj", "cube.tga")
{
    lookAt = glm::vec3(5.0f, 0.5f, -0.01f);

    moveFlags = MOVE_FLAG_NONE;

    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
}
