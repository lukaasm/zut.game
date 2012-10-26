/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "GameObject.h"

#include "RenderDevice.h"
#include "ResourcesMgr.h"

void GameObject::OnRender(RenderDevice* rd)
{
    RenderData* renderData = sResourcesMgr->GetRenderDataForModel(modelName);
    rd->DrawTriangles(renderData->vertexArray, 0 , renderData->size);
}