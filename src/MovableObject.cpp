#include "MovableObject.h"

#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Input.h"

void DynamicObject::Move()
{
    if (moveFlags & MOVE_FLAG_FORWARD)
    {
        glm::vec3 offset = lookDirection * moveInfos[MOVE_TYPE_FORWARD].speed;
        position += offset;
    }

    if (moveFlags & MOVE_FLAG_BACKWARD)
    {
        glm::vec3 offset = lookDirection * moveInfos[MOVE_TYPE_BACKWARD].speed;
        position -= offset;
    }

    if (moveFlags & MOVE_FLAG_UPWARD)
    {
        glm::vec3 offset = up * moveInfos[MOVE_TYPE_UPWARD].speed;
        position += offset;
    }

    if (moveFlags & MOVE_FLAG_DOWNWARD)
    {
        glm::vec3 offset = up * moveInfos[MOVE_TYPE_DOWNWARD].speed;
        position -= offset;
    }

    if (moveFlags & MOVE_FLAG_STRAFE_RIGHT)
    {
        glm::vec3 offset = right * moveInfos[MOVE_TYPE_STRAFE_RIGHT].speed;
        position += offset;
    }

    if (moveFlags & MOVE_FLAG_STRAFE_LEFT)
    {
        glm::vec3 offset = right * moveInfos[MOVE_TYPE_STRAFE_LEFT].speed;
        position -= offset;
    }

    if (moveFlags & MOVE_FLAG_ROTATE_LEFT)
    {
        lookDirection = glm::rotate(lookDirection, moveInfos[MOVE_TYPE_ROTATE_LEFT].speed, up);
        right = glm::normalize(glm::cross(lookDirection, up));
        rotationY += moveInfos[MOVE_TYPE_ROTATE_LEFT].speed;
    }

    if (moveFlags & MOVE_FLAG_ROTATE_RIGHT)
    {
        lookDirection = glm::rotate(lookDirection, -moveInfos[MOVE_TYPE_ROTATE_RIGHT].speed, up);
        right = glm::normalize(glm::cross(lookDirection, up));
        rotationY -= moveInfos[MOVE_TYPE_ROTATE_RIGHT].speed;
    }

    if (position.y < (scale.y/2))
        position.y = scale.y / 2;
}

void DynamicObject::OnUpdate(const uint32 & diff)
{
    Move();

    GameObject::OnUpdate(diff);
}

void DynamicObject::AddMoveType(MoveInfo flag)
{
    moveFlags = MoveFlags((moveFlags | flag.apply) & ~flag.remove);
}

void DynamicObject::ClearMoveType(MoveFlags flag)
{
    moveFlags = MoveFlags(moveFlags & ~flag);
}

DynamicObject::DynamicObject() : GameObject("cube.obj", "cube.tga")
{
    lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);

    moveFlags = MOVE_FLAG_NONE;

    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
}
