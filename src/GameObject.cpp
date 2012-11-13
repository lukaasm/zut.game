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

glm::mat4 GameObject::GetModelMatrix() const
{
    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}

void GameObject::SetPosition(const Position& pos)
{
    position = pos;
}

void GameObject::SetScale(const glm::vec3& scale)
{
    this->scale = scale;
}

void GameObject::SetGuid(uint32 guid)
{
    this->guid = guid;
}

void GameObject::SetBoundingObject(BoundingObject* object)
{
    boundingObject = object;
}
