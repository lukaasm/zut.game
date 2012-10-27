/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Grid.h"

#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "GameObject.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"
#include "SceneMgr.h"
#include "Shader.h"

using namespace glm;

Grid::Grid() : GameObject("grid", "")
{
}

void Grid::OnRender(RenderDevice* rd)
{
    // scale only local matrix
    modelMatrix = scale(mat4(1.0f), vec3(0.5f));

    RenderData* renderData = sResourcesMgr->GetRenderDataForModel(modelName);
    rd->DrawLines(renderData->vertexArray, 0, renderData->size);
}
