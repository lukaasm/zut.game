/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "GameObject.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderDevice.h"
#include "ResourcesMgr.h"

void GameObject::OnRender(RenderDevice* rd)
{
    rd->ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId(textureName));

    RenderData* renderData = sResourcesMgr->GetRenderDataForModel(modelName);
    rd->DrawTriangles(renderData->vertexArray, 0 , renderData->size);
}

glm::mat4 GameObject::GetModelMatrix()
{
    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
    //modelMatrix = glm::rotate(modelMatrix, rotateY, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}
