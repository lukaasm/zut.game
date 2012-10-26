#include "SceneMgr.h"

#include "Camera.h"

void SceneMgr::OnInit()
{
    SetCamera(new Camera());
}

void SceneMgr::OnUpdate(uint32 diff)
{
    GetCamera()->OnUpdate(diff);
}

void SceneMgr::OnRender(RenderDevice* rd)
{

}

void SceneMgr::SetCamera(Camera* camera)
{
    this->camera = camera;
}

SceneMgr::~SceneMgr()
{
    delete camera;
}
