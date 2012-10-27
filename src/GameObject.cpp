/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "GameObject.h"

#include <gl/glew.h>

#include "RenderDevice.h"
#include "ResourcesMgr.h"

void GameObject::OnRender(RenderDevice* rd)
{
    rd->ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId(textureName));

    RenderData* renderData = sResourcesMgr->GetRenderDataForModel(modelName);
    rd->DrawTriangles(renderData->vertexArray, 0 , renderData->size);
}
