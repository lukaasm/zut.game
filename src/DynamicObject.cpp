#include "DynamicObject.h"

#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "SceneMgr.h"

void DynamicObject::Move(const uint32& diff)
{
    Position original = position;
    if (moveFlags & MOVE_FLAG_FORWARD)
    {
        glm::vec3 offset = lookDirection * moveInfos[MOVE_TYPE_FORWARD].speed *(0.001f * diff);
        RedoMoveOnCollision(original, offset);
    }

    if (moveFlags & MOVE_FLAG_BACKWARD)
    {
        glm::vec3 offset = -lookDirection * moveInfos[MOVE_TYPE_BACKWARD].speed *(0.001f * diff);
        RedoMoveOnCollision(original, offset);
    }

    if (moveFlags & MOVE_FLAG_UPWARD)
    {
        glm::vec3 offset = up * moveInfos[MOVE_TYPE_UPWARD].speed *(0.001f * diff);
        RedoMoveOnCollision(original, offset);
    }

    if (moveFlags & MOVE_FLAG_DOWNWARD)
    {
        glm::vec3 offset = -up * moveInfos[MOVE_TYPE_DOWNWARD].speed *(0.001f * diff);
        RedoMoveOnCollision(original, offset);
    }

    if (moveFlags & MOVE_FLAG_STRAFE_RIGHT)
    {
        glm::vec3 offset = glm::cross(lookDirection, up) * moveInfos[MOVE_TYPE_STRAFE_RIGHT].speed *(0.001f * diff);
        RedoMoveOnCollision(original, offset);
    }

    if (moveFlags & MOVE_FLAG_STRAFE_LEFT)
    {
        glm::vec3 offset = -glm::cross(lookDirection, up) * moveInfos[MOVE_TYPE_STRAFE_LEFT].speed *(0.001f * diff);
        RedoMoveOnCollision(original, offset);
    }

    if (moveFlags & MOVE_FLAG_ROTATE_LEFT)
    {
        lookDirection = glm::rotate(lookDirection, moveInfos[MOVE_TYPE_ROTATE_LEFT].speed *(0.001f * diff), up);
        rotationY += moveInfos[MOVE_TYPE_ROTATE_LEFT].speed *(0.001f * diff);
    }

    if (moveFlags & MOVE_FLAG_ROTATE_RIGHT)
    {
        lookDirection = glm::rotate(lookDirection, -moveInfos[MOVE_TYPE_ROTATE_RIGHT].speed *(0.001f * diff), up);
        rotationY -= moveInfos[MOVE_TYPE_ROTATE_RIGHT].speed *(0.001f * diff);
    }

    //if (position.y < (scale.y/2))
    //    position.y = scale.y / 2;
    //else
        recreateModelMatrix();
}

void DynamicObject::OnUpdate(const uint32 & diff)
{
    Move(diff);

    GameObject::OnUpdate(diff);

    for (auto i = scripts.begin(); i != scripts.end(); ++i)
        (*i)(*this);
}

void DynamicObject::AddMoveType(MoveInfo flag)
{
    moveFlags = MoveFlags((moveFlags | flag.apply) & ~flag.remove);
}

void DynamicObject::ClearMoveType(MoveFlags flag)
{
    moveFlags = MoveFlags(moveFlags & ~flag);
}

DynamicObject::DynamicObject() : GameObject("mb.obj", "mb.tga")
{
    lookDirection = glm::vec3(0.0f, 0.0f, 1.0f);

    moveFlags = MOVE_FLAG_NONE;

    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void DynamicObject::RedoMoveOnCollision(Position& original, Position& offset)
{
    SetPosition(original + offset);

    sSceneMgr->CollisionTest(this);
    if (coll == 1.0f)
        SetPosition(original);
    else
        original += offset;
}
