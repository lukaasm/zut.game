/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Grid.h"

#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "GameObject.h"
#include "ModelData.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"
#include "SceneMgr.h"
#include "Shader.h"

Grid::Grid() : GameObject("grid", "")
{
    SetPosition(glm::vec3(-5.0f, 0.0f, 5.0f));
    // scale only local matrix
    SetScale(glm::vec3(0.5f));
}

void Grid::OnRender()
{
    ModelData* modelData = sResourcesMgr->GetModelData(modelName);
    OGLHelper::DrawLines(modelData->vao);
}
