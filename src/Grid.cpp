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

Grid::Grid() : GameObject()
{
    _shader = new Shader("../res/shaders/shader.vert", "../res/shaders/shader.frag");
}

Grid::~Grid()
{
    delete_ptr(_shader)
}

void Grid::OnRender(RenderDevice* rd)
{
    _shader->Bind();

    // scale only local matrix
    modelMatrix = scale(mat4(1.0f), vec3(0.5f));

    Camera* camera = sSceneMgr->GetCamera();
    rd->SetUniforms(_shader, modelMatrix, camera->GetProjMatrix(), camera->GetViewMatrix());

    RenderData* renderData = sResourcesMgr->GetRenderDataForModel("grid");
    rd->DrawLines(renderData->vertexArray, 0, 4*40);

    _shader->Unbind();
}
