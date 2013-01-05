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
#include "RenderDevice.h"
#include "ResourcesMgr.h"
#include "Shader.h"
#include "Timer.h"
#include "Terrain.h"

SceneMgr::SceneMgr()
{
}

void SceneMgr::OnInit()
{
    guid = 0;

    terrain = new Terrain();

    player = new DynamicObject("sphere.obj", "placeholder.tga");
    player->SetPosition(Position(15.0f, terrain->GetHeight(15.0f, 10.0f) + 0.075f, 10.0f));
    player->SetScale(glm::vec3(0.15f));
    player->SetBoundingObject(sResourcesMgr->GetModelData(player->GetModel())->boundingBox);
    player->scripts["OnUpdate"].push_back(
        [](DynamicObject& ob)
        {
            for (Keyboard::KeysMap::const_iterator i = sKeyboard->GetKeysMap().begin(); i != sKeyboard->GetKeysMap().end(); ++i)
            {
                const MoveInfo& info = Keyboard::Key2MoveInfo(i->first);
                if (info.apply == MOVE_FLAG_NONE)
                    continue;

                if (i->second)
                    ob.AddMoveType(info);
                else
                    ob.ClearMoveType(info.apply);
            }
            ob.GetPosition().y = sSceneMgr->GetHeight(&ob);
        });

    player->scripts["OnUpdate"].push_back(
        [](DynamicObject& ob)
        {
            if (!ob.createTime->Passed())
                return;

            ob.createTime->Start(200);

            auto it = sKeyboard->GetKeysMap().find(GLFW_KEY_LCTRL);
            if (it == sKeyboard->GetKeysMap().end() || it->second == false)
                return;

            DynamicObject* shoot = new DynamicObject("sphere.obj", "placeholder.tga");
            shoot->SetTypeId(TYPEID_PROJECTILE);
            shoot->SetPosition(ob.GetPosition());
            shoot->SetScale(glm::vec3(0.05f));
            shoot->AddMoveType(moveInfos[MOVE_TYPE_FORWARD]);
            shoot->SetRotationY(sSceneMgr->GetPlayer()->GetRotationY());

            shoot->scripts["OnUpdate"].push_back(
                [](DynamicObject& ob)
                {
                    ob.SetPosition(Position(ob.GetPosition().x, sSceneMgr->GetHeight(&ob) + 0.25f, ob.GetPosition().z));

                    if (ob.createTime->Elapsed() > 3000)
                        sSceneMgr->UnregisterObject(&ob);
                });

            PointLight light;
            light.owner = shoot;
            light.Color = glm::vec3(0.5f, 0.2f, 0.6f);
            light.Intensity = 1.0f;
            light.Radius = 8.0f;

            sSceneMgr->GetPointLights().push_back(light);
            sSceneMgr->RegisterObject(shoot);
        });

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

    ob = new DynamicObject("boid.obj", "placeholder.tga");
    ob->SetScale(glm::vec3(0.25f));
    ob->SetPosition(Position(14.25f, GetHeight(14.25f, 14.0f, ob), 14.0f));

    ((DynamicObject*)(ob))->scripts["OnUpdate"].push_back(
        [](DynamicObject& ob)
        {
            DynamicObject* player = sSceneMgr->GetPlayer();
            float dist = ob.GetDistance(player);
            if (dist < 0.5f || dist > 4.0f)
                ob.ClearMoveType(MOVE_FLAG_FORWARD);
            else if (dist < 3.0f)
            {
                ob.SetRotationY(ob.GetAngle(player));
                ob.AddMoveType(moveInfos[MOVE_TYPE_FORWARD]);
            }

            ob.GetPosition().y = sSceneMgr->GetHeight(&ob);
        });

    RegisterObject(ob);

    text2D.Init();

    initLights();
    deferred.Init();
}

void SceneMgr::OnUpdate(const uint32& diff)
{
    for (uint8 i = TYPEID_DYNAMIC; i < TYPEID_MAX; ++i)
    {
        for (auto it = objects[i].begin(); it != objects[i].end(); ++it)
            reinterpret_cast<DynamicObject*>(it->second)->OnUpdate(diff);
    }

    float time = glfwGetTime()*150.0f;
    uint8 x = 0;
    for (auto i = GetPointLights().begin(); i != GetPointLights().end();)
    {
        if (i->owner != nullptr)
        {
            uint32 ownerGuid = i->owner->GetGuid();
            if (std::find_if(unregisterQueue.begin(), unregisterQueue.end(), [&] (GameObject* ob) -> bool { return ob->GetGuid() == ownerGuid; }) != unregisterQueue.end())
            {
                i = GetPointLights().erase(i);
                continue;
            }

            i->Position = i->owner->GetPosition();
        }
        else
        {
            i->Position.y = 3.0f+x + (x % 2 ? sin((time/180)*PI)*(x + 0.5f) : cos((time/180)*PI)*(x + 0.5f));
            i->Position.x = 18.0f + cos((time/180)*PI)*2*(x + 0.5f)*.7f;
            i->Position.z = 12.5f + sin((time/180)*PI)*4*(x + 0.5f)*.7f;
        }
        ++i;
        ++x;
    }

    while (!unregisterQueue.empty())
    {
        GameObject* object = unregisterQueue.front();
        objects[object->GetTypeId()].erase(object->GetGuid());

        delete object;
        unregisterQueue.pop_front();
    }

    GetCamera()->OnUpdate(diff);
}

bool SceneMgr::CollisionTest(GameObject* object, ObjectTypeId type)
{
    AABoundingBox* bounds = object->GetBoundingObject();
    if (bounds == nullptr)
        return false;

    for (auto i = objects[type].begin(); i != objects[type].end(); ++i)
    {
        GameObject* ob = i->second;
        if (object == ob || ob->GetBoundingObject() == nullptr)
            continue;

        if (bounds->Intersection(*(ob->GetBoundingObject())))
        {
            if (object->GetTypeId() != TYPEID_STATIC)
                reinterpret_cast<DynamicObject*>(object)->OnCollision();
            if (ob->GetTypeId() != TYPEID_STATIC)
                reinterpret_cast<DynamicObject*>(ob)->OnCollision();

            return true;
        }
    }
    return false;
}

SceneMgr::~SceneMgr()
{
    GameObjectsMap map;
    for (uint8 i = TYPEID_STATIC; i < TYPEID_MAX; ++i)
    {
        map.insert(objects[i].begin(), objects[i].end());
        objects[i].clear();
    }

    std::for_each(map.begin(), map.end(), [](std::pair<uint32, GameObject*> p) { delete p.second; });
    std::for_each(cameras.begin(), cameras.end(), [](Camera* cam) { delete cam; });
}

void SceneMgr::RegisterObject(GameObject* object)
{
    uint32 GUID = ++guid;
    object->SetGuid(GUID);

    objects[object->GetTypeId()][GUID] = object;

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

float SceneMgr::GetHeight(float x, float z, GameObject* ob)
{
    float h = terrain->GetHeight(x, z);
    if (ob != nullptr)
        return h + sResourcesMgr->GetModelData(ob->GetModel())->height*ob->GetScale().y*0.5f;
    return h;
}

float SceneMgr::GetHeight(GameObject* ob)
{
    float h = terrain->GetHeight(ob->GetPosition().x, ob->GetPosition().z);
    return h + sResourcesMgr->GetModelData(ob->GetModel())->height*ob->GetScale().y*0.5f;
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

//     text2D.RenderSprite(5, 5, 200, deferred.depthTexture);
//     text2D.RenderSprite(5, 210, 200, deferred.colorTexture);
//     text2D.RenderSprite(5, 415, 200, deferred.normalTexture);
}

void SceneMgr::initLights()
{
    // point
    PointLight light;
    light.owner = nullptr;

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
    GameObjectsMap map;
    for (uint8 i = TYPEID_STATIC; i < TYPEID_MAX; ++i)
        map.insert(objects[i].begin(), objects[i].end());

    return map;
}

void SceneMgr::UnregisterObject(GameObject* object)
{
    if (AABoundingBox* bounds = object->GetBoundingObject())
        boundingBoxes.erase(bounds);

    unregisterQueue.push_back(object);
}
