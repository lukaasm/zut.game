#include "SceneMgr.h"

#include <gl/glew.h>
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
#include "Terrain.h"

void SceneMgr::OnInit()
{
    guid = 0;

    terrain = new Terrain();

    player = new Player();
    player->SetPosition(Position(15.0f, 0.075f, 10.0f));
    player->SetScale(glm::vec3(0.15f));
    player->SetBoundingObject(sResourcesMgr->GetModelData(player->GetModel())->boundingBox);
    RegisterObject(player);

    cameras.push_back(new TppCamera());
    cameras.push_back(new FppCamera());
    cameras.push_back(new EagleEyeCamera());

    std::for_each(cameras.begin(), cameras.end(), [this](Camera* c) { c->LinkTo(player); });

    currentCamera = 0;

    //RegisterObject(new Grid());

    GameObject* cube;

#define POPULATE_CUBE(a,b,c) cube = new GameObject("cube.obj", "cube.tga"); \
    cube->SetPosition(glm::vec3(a, b, c)); \
    cube->SetScale(glm::vec3(0.25f)); \
    RegisterObject(cube); \
    cube->SetBoundingObject(sResourcesMgr->GetModelData(cube->GetModel())->boundingBox);

    POPULATE_CUBE(14.25f, 0.25f, 15.0f)
    POPULATE_CUBE(15.75f, 0.25f, 15.0f)
    POPULATE_CUBE(14.625f, 0.875f, 15.0f)
    POPULATE_CUBE(15.375f, 0.875f, 15.0f)
    POPULATE_CUBE(15.0f, 0.875f+0.625f, 15.0f)

    DynamicObject* ccube = new DynamicObject();
    ccube->SetPosition(glm::vec3(15.0f, 0.875f+1.5f, 15.0f));
    ccube->SetScale(glm::vec3(0.25f));
    ccube->SetBoundingObject(sResourcesMgr->GetModelData(ccube->GetModel())->boundingBox);

    ccube->scripts.push_back([](DynamicObject& ob){ ob.coll ? ob.AddMoveType(moveInfos[MOVE_TYPE_FORWARD]) : ob.ClearMoveType(MOVE_FLAG_FORWARD); });
    ccube->AddMoveType(moveInfos[MOVE_TYPE_ROTATE_LEFT]);
    RegisterObject(ccube);

    text2D.Init();
}

void SceneMgr::OnUpdate(const uint32& diff)
{
    for (auto i = dynamicObjects.begin(); i != dynamicObjects.end(); ++i)
        i->second->OnUpdate(diff);

    GetCamera()->OnUpdate(diff);

    player->GetPosition().y = terrain->GetHeight(player->GetPosition().x, player->GetPosition().z);
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
            i->second->coll = 1.0f;
        }
        else
            i->second->coll = 0.0f;
    }
}

void SceneMgr::OnRender(RenderDevice* rd)
{
    bool renderTexture = sConfig->GetDefault("render.textures", true);
    bool renderBounds = sConfig->GetDefault("render.bounds", true);

    GameObjectsMap map = staticObjects;
    map.insert(dynamicObjects.begin(), dynamicObjects.end());

    Shader* shader = sResourcesMgr->GetShader("test.glsl");
    shader->Bind();

    glm::mat4 x = glm::mat4(1.0f); //glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, 0.0f, -10.0f));
    rd->SetUniforms(shader, GetCamera()->GetProjMatrix(), GetCamera()->GetViewMatrix(), x, 0.0f);
    terrain->OnRender(rd);

    shader->Unbind();

    shader = sResourcesMgr->GetShader("test.glsl");
    for (auto i = map.begin(); i != map.end(); ++i)
    {
        shader->Bind();

        GameObject* ob = i->second;

        rd->SetUniforms(shader, GetCamera()->GetProjMatrix(), GetCamera()->GetViewMatrix(), ob->GetModelMatrix(), renderTexture ? ob->IsTextured() : 0.0f);
        i->second->OnRender(rd);

        if (renderBounds)
        {
            if (AABoundingBox* bounds = ob->GetBoundingObject())
            {
                rd->SetUniforms(shader, GetCamera()->GetProjMatrix(), GetCamera()->GetViewMatrix(), bounds->GetModelMatrix(), ob->coll);
                bounds->OnRender(rd);
            }
        }
        shader->Unbind();
    }

    std::stringstream fps;
    fps << "FrameTime: " << std::setprecision(3) << BaseApp::frameTime << "ms" << " FPS: " << int(1000.0f/BaseApp::frameTime);

    text2D.Print(rd, fps.str(), 10, sConfig->GetDefault("height", 600) - 12, 12);
    text2D.Print(rd, "WSAD - movement, <- -> - rotate", 10, 10, 12);
    text2D.Print(rd, "K - change camera, L - toggle textures", 10, 25, 12);
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
