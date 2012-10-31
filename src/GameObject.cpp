/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "GameObject.h"

#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderDevice.h"
#include "ResourcesMgr.h"

void GameObject::OnRender(RenderDevice* rd)
{
    rd->ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId(textureName));

    RenderData* renderData = sResourcesMgr->GetRenderDataForModel(modelName);
    rd->DrawTriangles(renderData->vertexArray, 0 , renderData->size);
}

mat4 GameObject::GetModelMatrix()
{
    mat4 modelMatrix;
    modelMatrix = translate(mat4(1.0f), position);
    modelMatrix = rotate(modelMatrix, rotationX, vec3(1.0f, 0.0f, 0.0f));
    //modelMatrix = rotate(modelMatrix, rotateY, vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}
