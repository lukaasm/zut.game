#include "DynamicObject.h"

#include <glm/core/func_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "SceneMgr.h"
#include "Timer.h"

void DynamicObject::Move(const uint32& diff)
{
    float penalty = (this == sSceneMgr->GetPlayer() ? 1.0f : 0.6f);
    Position original = position;
    if (moveFlags & MOVE_FLAG_FORWARD)
    {
        glm::vec3 offset = lookDirection * moveInfos[MOVE_TYPE_FORWARD].speed *(0.001f * diff) * penalty;
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
        SetRotationY(moveInfos[MOVE_TYPE_ROTATE_LEFT].speed *(0.001f * diff) + rotationY);

    if (moveFlags & MOVE_FLAG_ROTATE_RIGHT)
        SetRotationY(-moveInfos[MOVE_TYPE_ROTATE_LEFT].speed *(0.001f * diff) + rotationY);

    recreateModelMatrix();
}

void DynamicObject::OnUpdate(const uint32 & diff)
{
    Move(diff);

    for (auto i = scripts["OnUpdate"].begin(); i != scripts["OnUpdate"].end(); ++i)
        (*i)(*this);
}

void DynamicObject::OnCollision()
{
    for (auto i = scripts["OnCollision"].begin(); i != scripts["OnCollision"].end(); ++i)
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
    createTime = new Timer();
    createTime->Start(0);

    lookDirection = glm::vec3(0.0f, 0.0f, 1.0f);

    moveFlags = MOVE_FLAG_NONE;

    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

DynamicObject::DynamicObject(std::string model, std::string texture) : GameObject(model, texture)
{
    createTime = new Timer();
    createTime->Start(0);

    lookDirection = glm::vec3(0.0f, 0.0f, 1.0f);

    moveFlags = MOVE_FLAG_NONE;

    up = glm::vec3(0.0f, 1.0f, 0.0f);

    SetTypeId(TYPEID_DYNAMIC);
}

void DynamicObject::RedoMoveOnCollision(Position& original, Position& offset)
{
    SetPosition(original + offset);

    if (sSceneMgr->CollisionTest(this, TYPEID_STATIC))
        SetPosition(original);
}

void DynamicObject::SetRotationY(float rotation)
{
    lookDirection = glm::rotate(lookDirection, rotation - rotationY, up);
    rotationY = rotation;

    recreateModelMatrix();
}

void DynamicObject::SetRotationX(float rotation)
{
    lookDirection = glm::rotate(up, rotation - rotationX, lookDirection);
    rotationX = rotation;

    recreateModelMatrix();
}

float DynamicObject::GetAngle(GameObject* ob)
{
    float dx = ob->GetPosition().x - GetPosition().x;
    float dz = ob->GetPosition().z - GetPosition().z;

    float ang = atan2(dz, dx) * 180 / PI;
    return -ang+90;
}

float DynamicObject::GetDistance(GameObject* ob)
{
    float dx = GetPosition().x - ob->GetPosition().x;
    float dy = GetPosition().y - ob->GetPosition().y;
    float dz = GetPosition().z - ob->GetPosition().z;
    return sqrt((dx*dx) + (dy*dy) + (dz*dz));
}

DynamicObject::~DynamicObject()
{
    delete createTime;
}
