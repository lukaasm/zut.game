#include "SceneMgr.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
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

#define POPULATE_CUBE(a,b,c) cube = new GameObject("cube.obj", "cube.tga"); \
    cube->SetPosition(glm::vec3(a, terrain->GetHeight(a, c) + b, c)); \
    cube->SetScale(glm::vec3(0.25f)); \
    cube->SetBoundingObject(sResourcesMgr->GetModelData(cube->GetModel())->boundingBox); \
    RegisterObject(cube); \

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

    //RegisterObject(new Grid());

    GameObject* cube;

    POPULATE_CUBE(14.25f, 0.25f, 15.0f)
    POPULATE_CUBE(15.75f, 0.25f, 15.0f)
    POPULATE_CUBE(14.625f, 0.875f, 15.0f)
    POPULATE_CUBE(15.375f, 0.875f, 15.0f)
    POPULATE_CUBE(15.0f, 0.875f+0.625f, 15.0f)

    DynamicObject* ccube = new DynamicObject();
    ccube->SetPosition(glm::vec3(15.0f, terrain->GetHeight(15.0f, 15.0f) + 0.875f+1.5f, 15.0f));
    ccube->SetScale(glm::vec3(0.25f));
    ccube->SetBoundingObject(sResourcesMgr->GetModelData(ccube->GetModel())->boundingBox);

    ccube->scripts.push_back([](DynamicObject& ob){ ob.coll ? ob.AddMoveType(moveInfos[MOVE_TYPE_FORWARD]) : ob.ClearMoveType(MOVE_FLAG_FORWARD); });
    ccube->AddMoveType(moveInfos[MOVE_TYPE_ROTATE_LEFT]);
    RegisterObject(ccube);

    text2D.Init();

    initLights();
    deferred.Init();
}

void SceneMgr::OnUpdate(const uint32& diff)
{
    for (auto i = dynamicObjects.begin(); i != dynamicObjects.end(); ++i)
        i->second->OnUpdate(diff);

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
    //renderPass();

    renderGUI();
}

/*
void SceneMgr::renderPass()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bool renderTexture = sConfig->GetDefault("render.textures", true);
    bool renderBounds = sConfig->GetDefault("render.bounds", true);

    Shader* shader = sResourcesMgr->GetShader("lighting.glsl");
    shader->Bind();

    glm::mat4 MV = GetCamera()->GetViewMatrix();
    shader->SetUniform("in_MVP", GetCamera()->GetProjMatrix()*MV);
    shader->SetUniform("in_MV", MV);
    shader->SetUniform("in_N", glm::inverseTranspose(glm::mat3(MV)));
    shader->SetUniform("in_V", GetCamera()->GetViewMatrix());
    shader->SetUniform("textureSampler", 0);

    shader->SetLightSources(lights);

    terrain->OnRender();

    GameObjectsMap allObjects = staticObjects;
    allObjects.insert(dynamicObjects.begin(), dynamicObjects.end());
    for (auto i = allObjects.begin(); i != allObjects.end(); ++i)
    {
        GameObject* ob = i->second;

        glm::mat4 MV = GetCamera()->GetViewMatrix() * ob->GetModelMatrix();
        shader->SetUniform("in_MVP", GetCamera()->GetProjMatrix()*MV);
        shader->SetUniform("in_MV", MV);
        shader->SetUniform("in_N", glm::inverseTranspose(glm::mat3(MV)));

        shader->SetUniform("textureSampler", 0);
        OGLHelper::ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId(ob->GetTexture()));

        ob->OnRender();
    }

    for (uint8 i = 0; i < lights.size(); ++i)
    {
        glm::mat4 worldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(lights[i].Position));
       
	    worldMatrix = glm::scale(worldMatrix, glm::vec3(0.15));

        MV = GetCamera()->GetViewMatrix() * worldMatrix;
        shader->SetUniform("in_MVP", GetCamera()->GetProjMatrix() * MV);
        shader->SetUniform("in_MV", MV);
        shader->SetUniform("in_N", glm::inverseTranspose(glm::mat3(MV)));

        shader->SetUniform("textureSampler", 0);

	    ModelData* modelData = sResourcesMgr->GetModelData("sphere.obj");
        OGLHelper::ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId("placeholder.tga"));
        OGLHelper::DrawTriangles(modelData->vao);
    }

    shader->Unbind();

    if (renderBounds)
    {
        shader = sResourcesMgr->GetShader("simple.glsl");
        shader->Bind();
        for (auto i = boundingBoxes.begin(); i != boundingBoxes.end(); ++i)
        {
            AABoundingBox* bounds = (*i);
            glm::mat4 MV = GetCamera()->GetViewMatrix() * bounds->GetModelMatrix();
            shader->SetUniform("in_MVP", GetCamera()->GetProjMatrix()*MV);

            bounds->OnRender();
        }
        shader->Unbind();
    }
}
*/

void SceneMgr::renderGUI()
{
    std::stringstream fps;
    fps << "FrameTime: " << std::setprecision(3) << BaseApp::frameTime << "ms" << " FPS: " << BaseApp::fps;

    text2D.RenderText(fps.str(), 10, sConfig->GetDefault("height", 600) - 12, 12);
    text2D.RenderText("WSAD - movement, <- -> - rotate", 10, 10, 12);
    text2D.RenderText("K - change camera, L - toggle textures", 10, 25, 12);

    //text2D.RenderSprite(5, 5, 200, deferred.lightTexture);
}

void SceneMgr::initLights()
{
    // point
    PointLight light;
    light.Position = glm::vec3(18.0, 3.0, 15.5);
    light.Color = glm::vec3(1.0, 0.0, 0.0);
    light.Radius = 5.0f;
    light.Intensity = 1.0f;

    lights.push_back(light);

    light.Position = glm::vec3(12.0, 3.0, 15.5),
    light.Color = glm::vec3(0.0, 1.0, 0.0);
    light.Radius = 5.0f;
    light.Intensity = 1.0f;

    lights.push_back(light);

    light.Position = glm::vec3(15.0, 3.0, 12.5),
    light.Color = glm::vec3(0.0, 0.0, 1.0);
    light.Radius = 6.0f;
    light.Intensity = 1.0f;

    lights.push_back(light);

    light.Position = glm::vec3(15.0, 7.0, 15.5),
    light.Color = glm::vec3(1.0, 1.0, 1.0);
    light.Radius = 10.0f;
    light.Intensity = 2.0f;

    lights.push_back(light);

    light.Position = glm::vec3(15.0, 7.0, 35.0f),
    light.Color = glm::vec3(1.0, 0.0, 1.0);
    light.Radius = 10.0f;
    light.Intensity = 2.0f;

    lights.push_back(light);
}

GameObjectsMap SceneMgr::GetObjects()
{
    GameObjectsMap allObjects = staticObjects;
    allObjects.insert(dynamicObjects.begin(), dynamicObjects.end());

    return allObjects;
}
