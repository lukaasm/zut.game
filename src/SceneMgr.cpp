#include "SceneMgr.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iomanip>

#include "BaseApp.h"
#include "BoundingObject.h"
#include "Camera.h"
#include "Config.h"
#include "DynamicObject.h"
#include "GameObject.h"
#include "Grid.h"
#include "Input.h"
#include "ModelData.h"
#include "Player.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"
#include "Shader.h"
#include "Timer.h"
#include "Terrain.h"

void SceneMgr::OnInit()
{
    guid = 0;

    terrain = new Terrain();

    player = new Player();
    player->SetPosition(Position(15.0f, terrain->GetHeight(15.0f, 10.0f) + 0.075f, 10.0f));
    player->SetScale(glm::vec3(0.15f));
    player->SetBoundingObject(sResourcesMgr->GetModelData(player->GetModel())->boundingBox);
    RegisterObject(player);

    cameras.push_back(new TppCamera());
    cameras.push_back(new FppCamera());
    cameras.push_back(new EagleEyeCamera());

    std::for_each(cameras.begin(), cameras.end(), [this](Camera* c) { c->LinkTo(player); });

    currentCamera = 0;

    skybox = new GameObject("cube.obj", "skybox.tga");
    skybox->SetPosition(Position(0,0,0));
    skybox->SetScale(glm::vec3(200));

    GameObject* ob = new GameObject("ruiny.obj", "placeholder.tga");
    ob->SetPosition(Position(14.25f,0.25f,19.0f));
    ob->SetScale(glm::vec3(1));
    RegisterObject(ob);

    ob = new GameObject("shrooms.obj", "placeholder.tga");
    ob->SetPosition(Position(14.25f,0.25f,17.0f));
    ob->SetScale(glm::vec3(6));
    RegisterObject(ob);

    text2D.Init();

    initLights();
    deferred.Init();

    stuff =0;
}

void SceneMgr::OnUpdate(const uint32& diff)
{
    for (auto i = dynamicObjects.begin(); i != dynamicObjects.end(); ++i)
        i->second->OnUpdate(diff);

    stuff += diff*0.1;
    for (uint8 i = 0; i < GetPointLights().size(); ++i)
    {
        GetPointLights()[i].Position.y = 2.0f + sin((stuff/180)*3.14)*i;
        GetPointLights()[i].Position.x = 18.0f + cos((stuff/180)*3.14)*2*i*.7f;
        GetPointLights()[i].Position.z = 14.5f + sin((stuff/180)*3.14)*4*i*.7f;
    }


    GetCamera()->OnUpdate(diff);
}

void SceneMgr::CollisionTest(GameObject* object)
{
    object->coll = 0.0f;

    AABoundingBox* bounds = object->GetBoundingObject();

    GameObjectsMap map = staticObjects;
    map.insert(dynamicObjects.begin(), dynamicObjects.end());

    for (auto i = map.begin(); i != map.end(); ++i)
    {
        GameObject* ob = i->second;
        if (object == ob || ob->GetBoundingObject() == nullptr)
            continue;

        if (bounds->Intersection(*(ob->GetBoundingObject())))
        {
            object->coll = 1.0f;
            ob->coll = 1.0f;
        }
        else
            ob->coll = 0.0f;
    }
}

SceneMgr::~SceneMgr()
{
    GameObjectsMap map = staticObjects;
    map.insert(dynamicObjects.begin(), dynamicObjects.end());

    staticObjects.clear();
    dynamicObjects.clear();

    std::for_each(map.begin(), map.end(), [](std::pair<uint32, GameObject*> p) { delete p.second; });
    std::for_each(cameras.begin(), cameras.end(), [](Camera* cam) { delete cam; });
}

void SceneMgr::RegisterObject(GameObject* object)
{
    uint32 GUID = ++guid;
    object->SetGuid(GUID);

    if (object->IsDynamicObject())
        dynamicObjects[GUID] = object;
    else
        staticObjects[GUID] = object;

    if (AABoundingBox* bounds = object->GetBoundingObject())
        boundingBoxes.insert(bounds);
}

Camera* SceneMgr::GetCamera()
{
    if (cameras.empty())
        return nullptr;

    return cameras[currentCamera];
}

void SceneMgr::ToggleCamera()
{
    currentCamera = ++currentCamera % cameras.size();
}

void SceneMgr::OnResize(uint32 width, uint32 height)
{
    for (auto i = cameras.begin(); i != cameras.end(); ++i)
        (*i)->OnResize(width, height);
}

float SceneMgr::GetHeight(float x, float z)
{
    return terrain->GetHeight(x, z);
}

void SceneMgr::OnRender()
{
    deferred.GeometryPass();
    deferred.LightsPass();
    deferred.FinalPass();

    renderGUI();
}

void SceneMgr::renderGUI()
{
    std::stringstream fps;
    fps << "FrameTime: " << std::setprecision(3) << BaseApp::frameTime << "ms" << " FPS: " << BaseApp::fps;

    text2D.RenderText(fps.str(), 10, sConfig->GetDefault("height", 600) - 12, 12);
    text2D.RenderText("WSAD - movement, <- -> - rotate", 10, 10, 12);
    text2D.RenderText("K - change camera, L - toggle textures", 10, 25, 12);

    //text2D.RenderSprite(5, 5, 200, deferred.depthTexture);
}

void SceneMgr::initLights()
{
    // point
    PointLight light;
    light.Position = glm::vec3(18.0, 3.0, 15.5);
    light.Color = glm::vec3(1.0, 0.0, 0.0);
    light.Radius = 4.0f;
    light.Intensity = 1.0f;

    lights.push_back(light);

    light.Position = glm::vec3(12.0, 1.5, 15.5),
    light.Color = glm::vec3(0.0, 1.0, 0.0);
    light.Radius = 3.0f;
    light.Intensity = 1.0f;

    lights.push_back(light);

    light.Position = glm::vec3(15.0, 3.0, 16.5),
    light.Color = glm::vec3(0.0, 0.0, 1.0);
    light.Radius = 4.0f;
    light.Intensity = 2.0f;

    lights.push_back(light);
}

GameObjectsMap SceneMgr::GetObjects()
{
    GameObjectsMap allObjects = staticObjects;
    allObjects.insert(dynamicObjects.begin(), dynamicObjects.end());

    return allObjects;
}
