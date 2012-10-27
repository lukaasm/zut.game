/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "GameObject.h"

#include <gl/glew.h>

#include "RenderDevice.h"
#include "ResourcesMgr.h"

void GameObject::OnRender(RenderDevice* rd)
{
    if (modelName.find("square") != std::string::npos)
    {
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, sResourcesMgr->GetTextureId("test"));
    }

    RenderData* renderData = sResourcesMgr->GetRenderDataForModel(modelName);
    rd->DrawTriangles(renderData->vertexArray, 0 , renderData->size);
}
