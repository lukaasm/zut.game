/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "GameObject.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ModelData.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"

void GameObject::OnRender(RenderDevice* rd)
{
    rd->ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId(textureName));

    ModelData* modelData = sResourcesMgr->GetModelData(modelName);
    rd->DrawTriangles(modelData->vao);
}

void GameObject::recreateAllMatrixes()
{
    recreateModelMatrix();
    recreateAAModelMatrix();
}

void GameObject::recreateModelMatrix()
{
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
}

void GameObject::recreateAAModelMatrix()
{
    aaModelMatrix = glm::translate(glm::mat4(1.0f), position);
    aaModelMatrix = glm::scale(aaModelMatrix, scale);
}

const glm::mat4& GameObject::GetModelMatrix() const
{
    return modelMatrix;
}

const glm::mat4& GameObject::GetAAModelMatrix() const
{
    return aaModelMatrix;
}

void GameObject::SetPosition(const Position& pos)
{
    position = pos;
    recreateAllMatrixes();
}

void GameObject::ChangePosition(const glm::vec3& offset)
{
    position += offset;
    modelMatrix = glm::translate(modelMatrix, offset/scale);
    aaModelMatrix = glm::translate(aaModelMatrix, offset/scale);
}

void GameObject::SetScale(const glm::vec3& scale)
{
    this->scale = scale;
    recreateAllMatrixes();
}

void GameObject::SetGuid(uint32 guid)
{
    this->guid = guid;
}

void GameObject::SetBoundingObject(BoundingObject* object)
{
    boundingObject = object;
}

GameObject::GameObject(std::string model, std::string texture) : coll(0.0f), modelName(model), textureName(texture), rotationX(0.0f), rotationY(0.0f), boundingObject(nullptr)
{
    recreateAllMatrixes();
}
