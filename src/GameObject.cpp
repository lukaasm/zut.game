/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "GameObject.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ModelData.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"

void GameObject::OnRender()
{
    //rd->ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId(textureName));

    ModelData* modelData = sResourcesMgr->GetModelData(modelName);
    OGLHelper::DrawTriangles(modelData->vao);
}

void GameObject::recreateModelMatrix()
{
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
}

const glm::mat4& GameObject::GetModelMatrix() const
{
    return modelMatrix;
}

void GameObject::SetPosition(const Position& pos)
{
    if (pos == position)
        return;

    position = pos;

    recreateModelMatrix();

    if (AABoundingBox* bounds = GetBoundingObject())
        bounds->RecalculateModelMatrix(GetPosition(), GetScale());
}

void GameObject::SetScale(const glm::vec3& scale)
{
    if (this->scale == scale)
        return;

    this->scale = scale;

    recreateModelMatrix();

    if (AABoundingBox* bounds = GetBoundingObject())
        bounds->RecalculateModelMatrix(GetPosition(), GetScale());
}

void GameObject::SetGuid(uint32 guid)
{
    this->guid = guid;
}

void GameObject::SetBoundingObject(BoundingBoxProto* object)
{
    boundingBox = new AABoundingBox(*object, this);
}

GameObject::GameObject(std::string model, std::string texture) : coll(0.0f), modelName(model), textureName(texture), rotationX(0.0f), rotationY(0.0f), boundingBox(nullptr)
{
}

AABoundingBox::AABoundingBox(const BoundingBoxProto& proto, GameObject* o) : owner(o)
{
    vao = proto.vao;

    vao.releaseGPUResources = false;

    BoundingBoxProto::min = proto.min;
    BoundingBoxProto::max = proto.max;

    RecalculateModelMatrix(owner->GetPosition(), owner->GetScale());
}

void AABoundingBox::RecalculateModelMatrix(const Position& pos, const glm::vec3& scale)
{
    modelMatrix = glm::translate(glm::mat4(1.0f), pos);
    modelMatrix = glm::scale(modelMatrix, scale);

    AABoundingBox::min = glm::vec3(modelMatrix * glm::vec4(BoundingBoxProto::min, 1.0f));
    AABoundingBox::max = glm::vec3(modelMatrix * glm::vec4(BoundingBoxProto::max, 1.0f));
}

bool AABoundingBox::Intersection(const AABoundingBox& b)
{
    if (GetMin().x > b.GetMax().x)
        return false;

    if (b.GetMin().x > GetMax().x)
        return false;

    if (GetMin().y > b.GetMax().y)
        return false;

    if (b.GetMin().y > GetMax().y)
        return false;

    if (GetMin().z > b.GetMax().z)
        return false;

    if (b.GetMin().z > GetMax().z)
        return false;

    return true;
}
