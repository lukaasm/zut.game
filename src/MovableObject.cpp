#include "MovableObject.h"

#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Input.h"

/// FIX ALL VECTOR CALCULATIONS ( UP,RIGHT etc ) !!!!!!!!!!
void Player::Move()
{

    if (moveFlags & MOVE_FLAG_FORWARD)
    {
        glm::vec3 offset = glm::normalize(lookAt - position) * moveInfos[MOVE_TYPE_FORWARD].speed;
        position += offset;
        lookAt += offset;
    }

    if (moveFlags & MOVE_FLAG_BACKWARD)
    {
        glm::vec3 offset = glm::normalize(lookAt - position) * moveInfos[MOVE_TYPE_BACKWARD].speed;
        position -= offset;
        lookAt -= offset;
    }

    if (moveFlags & MOVE_FLAG_UPWARD)
    {
        glm::vec3 offset = glm::normalize(up) * moveInfos[MOVE_TYPE_UPWARD].speed;
        position += offset;
        lookAt += offset;
    }

    if (moveFlags & MOVE_FLAG_DOWNWARD)
    {
        glm::vec3 offset = glm::normalize(up) * moveInfos[MOVE_TYPE_DOWNWARD].speed;
        position -= offset;
        lookAt -= offset;
    }

    if (moveFlags & MOVE_FLAG_STRAFE_RIGHT)
    {
        glm::vec3 offset = glm::normalize(right) * moveInfos[MOVE_TYPE_STRAFE_RIGHT].speed;
        position += offset;
        lookAt += offset;
    }

    if (moveFlags & MOVE_FLAG_STRAFE_LEFT)
    {
        glm::vec3 offset = glm::normalize(right) * moveInfos[MOVE_TYPE_STRAFE_LEFT].speed;
        position -= offset;
        lookAt -= offset;
    }

    if (moveFlags & MOVE_FLAG_ROTATE_LEFT)
    {
        lookAt = glm::rotate(lookAt, moveInfos[MOVE_TYPE_ROTATE_LEFT].speed, up);
        right = glm::cross(lookAt, up);
    }

    if (moveFlags & MOVE_FLAG_ROTATE_RIGHT)
    {
        lookAt = glm::rotate(lookAt, -moveInfos[MOVE_TYPE_ROTATE_RIGHT].speed, up);
        right = glm::cross(lookAt, up);
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

    Move();
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
