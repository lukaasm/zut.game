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
#include "Random.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"
#include "Shader.h"
#include "Timer.h"
#include "Terrain.h"

#include "Macros.h"

SceneMgr::SceneMgr()
{
}

#include <iostream>
void SceneMgr::OnInit()
{
    guid = 0;

    terrain = new Terrain();

    player = new DynamicObject("sphere.obj", "placeholder.tga");

    player->SETPOSITION2(18.06f, 6.6f, 0.075f, player);
    player->SetScale(glm::vec3(0.15f));
    player->EnableBoundingBox();
    player->scripts["OnUpdate"].push_back(
        [](DynamicObject& ob)
        {
            if (sMouse->IsButtonPressed(GLFW_MOUSE_BUTTON_1))
            {   
                uint32 x = sMouse->GetPos().x;
                uint32 centerX = sConfig->GetDefault("width", WINDOW_WIDTH) * 0.5f;

                int32 deltax = centerX - x;
                ob.SetOrientation(ob.GetOrientation() + deltax*0.3f);

                sMouse->SetPos(centerX, sMouse->GetPos().y);
                ob.AddMoveType(moveInfos[MOVE_TYPE_FORWARD]);
            }
            else
                ob.ClearMoveType(MOVE_FLAG_FORWARD);

            ob.GetPosition().y = sSceneMgr->GetHeight(&ob);
        });

    player->scripts["OnUpdate"].push_back(
        [](DynamicObject& ob)
        {
            if (!ob.createTime->Passed())
                return;

            ob.createTime->Start(200);

            if (!sMouse->IsButtonPressed(GLFW_MOUSE_BUTTON_2))
                return;

            DynamicObject* shoot = new DynamicObject("sphere.obj", "placeholder.tga");
            shoot->SetTypeId(TYPEID_PROJECTILE);
            shoot->SetPosition(ob.GetPosition());
            shoot->SetScale(glm::vec3(0.05f));
            shoot->AddMoveType(moveInfos[MOVE_TYPE_FORWARD]);
            shoot->SetOrientation(sSceneMgr->GetPlayer()->GetOrientation());
            shoot->EnableBoundingBox();

            shoot->scripts["OnUpdate"].push_back(
                [](DynamicObject& ob)
                {
                    ob.SetPosition(Position(ob.GetPosition().x, sSceneMgr->GetHeight(&ob) + 0.25f, ob.GetPosition().z));

                    if (ob.createTime->Elapsed() > 3000)
                    {
                         sSceneMgr->UnregisterObject(&ob);
                         return;
                    }
                    sSceneMgr->CollisionTest(&ob, TYPEID_DYNAMIC);
                });

            shoot->scripts["OnCollision"].push_back(
                [](DynamicObject& ob)
                {
                    sSceneMgr->UnregisterObject(&ob);
                });

            PointLight light;
            light.owner = shoot;
            light.Color = glm::vec3(sRandom->Float(0.0f, 1.0f), sRandom->Float(0.0f, 1.0f), sRandom->Float(0.0f, 1.0f));
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

    GameObject* ob;
    ADDSTATICOBJECT(true, "wall.obj", "wall.tga", 14.25f, 19.0f, 0.1f)
    ADDSTATICOBJECT(true, "wall.obj", "wall.tga", 10.15f, 19.0f, 0.2f)
    ob->SetOrientation(180.0f);
    ADDSTATICOBJECT(true, "wall.obj", "wall.tga", 6.75f, 19.0f, 0.6f)
    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga", 16.03f, 10.59f, 1.0f)

    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga",24.9106, 8.30256, 0.7f)
    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga",24.8112, 14.6395, 0.1f)
    ADDSTATICOBJECT(true, "wall.obj", "wall.tga", 19.8748, 19.3333, 0.2f)
    ADDSTATICOBJECT(true, "wall.obj", "wall.tga", 24.2592, 19.6117, 0.5f)
    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga", 18.9083, 32.7982, 0.2f)
    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga", 17.6757, 36.7732, 0.5f)
    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga", 45.7446, 29.6961, 0.2f)
    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga", 45.296, 33.4755, 0.2f)

    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga", 24.8112, 17.5679, -1.0f)
    ob->SetOrientation(180.0f);
    ob->SetScale(glm::vec3(1.0f, 0.6f, 1.0f));

    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga", 35.1509, 39.3359, 0.2f)
    ADDSTATICOBJECT(true, "wall.obj", "wall.tga", 27.818, 47.0233, 0.6f)
    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga", 36.7148, 50.5978, 0.7f)
    // veg
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 11.49f, 20.18f, 0.25f)
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 21.48f, 13.66f, 0.25f)
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 30.97f, 20.42f, 0.25f)
    ob->SetScale(glm::vec3(sRandom->Float(1.0f, 3.0f)));
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 37.84f, 14.32f, 0.25f)
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 44.33f, 24.55f, 0.25f)
    ob->SetScale(glm::vec3(sRandom->Float(1.0f, 3.0f)));
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 38.92f, 39.92f, 0.25f)
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 45.56f, 46.99f, 0.25f)
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 25.52f, 55.84f, 0.25f)
    ob->SetScale(glm::vec3(sRandom->Float(1.0f, 3.0f)));
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 12.95f, 44.97f, 0.25f)
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 14.94f, 30.96f, 0.25f)
    ob->SetScale(glm::vec3(sRandom->Float(1.0f, 3.0f)));
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", 21.75f, 38.34f, 0.25f)
    ob->SetScale(glm::vec3(sRandom->Float(1.0f, 3.0f)));
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));

    ADDSTATICOBJECT(true, "rock.obj", "rock.tga", 17.7628, 14.4274, 0.15f)
    ob->SetScale(glm::vec3(sRandom->Float(0.5f, 1.0f)));
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f));

    ADDROCK(29.9952, 30.689)
    ADDROCK(43.1135, 30.9455)
    ADDROCK(42.7775, 38.8566)
    ADDROCK(49.0487, 43.5409)
    ADDROCK(36.1281, 48.1355)
    ADDROCK(29.9758, 45.7777)
    ADDROCK(26.3484, 45.8505)
    ADDROCK(24.455, 42.3597)
    ADDROCK(14.4008, 42.254)

    const float grasspos[] = 
    {
        21.1517, 28.2122,
        19.9492, 31.1298,
        29.21, 37.6051,
        34.2774, 37.288,
        34.3717, 38.2049,
        34.3779, 40.2299,
        29.4939, 44.1679,
        27.7639, 44.0173,
        24.5768, 43.7519,
        15.1614, 44.0865,
        13.5919, 44.4032,
        13.7297, 46.6715,
        11.8539, 46.6634,
        38.496, 30.0358,
        37.418, 25.8203,
        37.2414, 19.2573,
        39.4801, 19.0879,
        42.9467, 20.1163,
        45.5441, 18.6061,
        48.5992, 14.09,
        48.2664, 9.61401,
        44.9637, 7.27924,
        40.8234, 7.96901,
        36.8172, 11.9557,
        33.7555, 10.6893,
        30.3859, 11.8092,
        27.8028, 6.263,
        12.137, 8.85481,
        8.91093, 9.54438,
        8.24701, 18.1648,
        12.4358, 18.1278,
        14.8884, 17.6661,
        22.0467, 15.9068,
        24.0351, 20.9162,
        30.8008, 20.3564,
        35.4133, 15.4151,
        37.3119, 14.4491,
        25.8543, 9.8331,
        25.555, 10.2371,
        18.4818, 20.1932,
        17.2443, 19.2313,
        15.8724, 19.8522,
        16.2512, 18.9779,
        14.7978, 20.0478,
        11.9917, 20.1321,
        10.8108, 20.2764,
        9.45813, 20.4047,
        9.77445, 21.1523,
        10.0704, 20.7388,
        26.7872, 29.4424,
        26.8996, 30.7081,
        27.4778, 31.9247,
        29.0158, 33.0937,
        30.6965, 33.1974,
        32.2988, 32.263,
        33.3795, 30.8634,
        33.4121, 29.0254,
        32.7729, 27.9318,
        31.3879, 26.9796,
        29.7955, 26.834,
        28.748, 27.1648,
        15.2598, 11.683,
        16.0369, 12.5239,
        16.9185, 11.8204,
        23.8699, 15.5878,
        23.9065, 16.4349,
        26.7004, 20.7079,
        28.1917, 27.474,
        27.4664, 28.6171,
        28.1465, 32.5388,
        29.646, 33.7162,
        31.9584, 32.4948,
        33.5225, 30.0371,
        24.8572, 34.3032,
        22.1164, 38.0635,
        15.0945, 31.4079,
        11.1283, 33.529,
        18.5864, 34.9631,
        18.5453, 35.907,
        18.5706, 37.3057,
        18.5681, 38.3715,
        15.1488, 24.9195,
        13.4782, 27.0705,
        11.3313, 27.0997,
        6.31634, 30.642,
        43.4588, 36.9663,
        44.6721, 39.7972,
        47.4991, 41.937,
        47.4968, 43.8374,
        45.3764, 46.3858,
        43.2497, 49.5892,
        41.055, 49.9066,
        38.3513, 49.7052,
        38.3015, 47.0158,
        34.9955, 45.9238,
        29.7808, 46.7894,
        40.9618, 32.434,
        40.9618, 32.434,
        43.8391, 33.1426,
        45.7706, 35.4448,
        48.452, 25.3982,
        49.8427, 23.059,
        53.6822, 17.6619,
        45.4787, 15.0787,
    };

    for (uint8 i = 0; i < sizeof(grasspos)/sizeof(float); i = i+2)
    {
        ADDGRASS(grasspos[i], grasspos[i+1]);
    }

    const float shroomspos[] =
    {
        38.6467, 29.7983,
        43.7451, 37.3669,
        46.4159, 32.6552,
        52.6209, 21.7367,
        48.2106, 13.7499,
        36.7028, 11.7624,
        27.9117, 6.19077,
        24.4608, 5.84988,
        12.3197, 9.13373,
        14.25f, 17.0f,
        35.4194, 15.3119,
        30.8599, 28.0123,
        28.1801, 28.3001,
        24.9416, 33.9931,
        18.591, 37.4832
    };

    for (uint8 i = 0; i < sizeof(shroomspos)/sizeof(float); i = i+2)
    {
        ADDSHROOMS(shroomspos[i], shroomspos[i+1]);
    }
//     ob = new DynamicObject("coin.obj", "coin.tga");
//     ob->SetPosition(Position(20.9049f, 0.0f, 17.905f));
//     ob->SetScale(glm::vec3(0.55));
//     ob->SetTypeId(TYPEID_COIN);
//     ob->EnableBoundingBox();

//     ((DynamicObject*)ob)->scripts["OnUpdate"].push_back(
//         [](DynamicObject& ob)
//         {
//             ob.AddMoveType(moveInfos[MOVE_TYPE_ROTATE_LEFT]);
//         }
//     );
// 
//     RegisterObject(ob);

    ob = new GameObject("palm.obj", "palm.tga");
    ob->SETPOSITION(11.855f, 8.93753f, 0.0f);
    ob->SetScale(glm::vec3(1.2f));
    RegisterObject(ob);

    ob = new DynamicObject("boid.obj", "boid.tga");
    ob->SetScale(glm::vec3(0.25f));
    ob->SETPOSITION(14.25f, 14.0f, 0.35f);
    ob->EnableBoundingBox();

    ENEMYSCRIPT
    RegisterObject(ob);

    ob = new DynamicObject("boid.obj", "boid.tga");
    ob->SetScale(glm::vec3(0.35f));
    ob->SETPOSITION(18.25f, 12.0f, 0.55f);
    ob->EnableBoundingBox();

    ENEMYSCRIPT
    RegisterObject(ob);

    ob = new DynamicObject("boid.obj", "boid.tga");
    ob->SetScale(glm::vec3(0.35f));
    ob->SETPOSITION(21.25f, 11.0f, 0.55f);
    ob->EnableBoundingBox();

    ENEMYSCRIPT
    RegisterObject(ob);

    ob = new DynamicObject("boid.obj", "boid.tga");
    ob->SetScale(glm::vec3(0.25f));
    ob->SETPOSITION(11.25f, 14.0f, 0.35f);
    ob->EnableBoundingBox();

    ENEMYSCRIPT
    RegisterObject(ob);

    ob = new DynamicObject("boid.obj", "boid.tga");
    ob->SetScale(glm::vec3(0.25f));
    ob->SETPOSITION(20.4f, 17.99f, 0.35f);
    ob->EnableBoundingBox();

    ENEMYSCRIPT
    RegisterObject(ob);

    ob = new DynamicObject("boid.obj", "boid.tga");
    ob->SetScale(glm::vec3(0.25f));
    ob->SETPOSITION(27.06f, 15.49f, 0.35f);
    ob->EnableBoundingBox();

    ENEMYSCRIPT
    RegisterObject(ob);

    ob = new DynamicObject("boid.obj", "boid.tga");
    ob->SetScale(glm::vec3(0.25f));
    ob->SETPOSITION(7.26f, 14.719f, 0.35f);
    ob->EnableBoundingBox();

    ENEMYSCRIPT
    RegisterObject(ob);

    ob = new DynamicObject("boid.obj", "boid.tga");
    ob->SetScale(glm::vec3(0.25f));
    ob->SETPOSITION(8.11f, 8.82f, 0.35f);
    ob->EnableBoundingBox();

    ENEMYSCRIPT
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
            if (i->owner == player)
            {
                ++i;
                continue;
            }

            uint32 ownerGuid = i->owner->GetGuid();
            if (std::find_if(unregisterQueue.begin(), unregisterQueue.end(), [&] (GameObject* ob) -> bool { return ob->GetGuid() == ownerGuid; }) != unregisterQueue.end())
            {
                i = GetPointLights().erase(i);
                continue;
            }

            i->Position = i->owner->GetPosition();
            if (i->owner->GetTypeId() == TYPEID_COIN)
            {
                i->Position.x += cos((time/150)*PI)*0.1f;
                i->Position.z += sin((time/150)*PI)*0.1f;
            }
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
        
        if (object->GetTypeId() == TYPEID_PROJECTILE && ob == player)
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
    float height = terrain->GetHeight(x, z);
    if (ob != nullptr)
    {
         ModelData* md = sResourcesMgr->GetModelData(ob->GetModel());
         BoundingBoxProto& bbp = md->boundingBox[0];
         height += (bbp.GetMax().y - bbp.GetMin().y) * ob->GetScale().y * 0.5f;
    }
    return height;
}

float SceneMgr::GetHeight(GameObject* ob)
{
    float height = terrain->GetHeight(ob->GetPosition().x, ob->GetPosition().z);
    ModelData* md = sResourcesMgr->GetModelData(ob->GetModel());
    BoundingBoxProto& bbp = md->boundingBox[0];
    height += (bbp.GetMax().y - bbp.GetMin().y) * ob->GetScale().y * 0.5f;
    return height;
}

void SceneMgr::OnRender()
{
    deferred.GeometryPass();
    //sMouse->ScreenToWorld(deferred.frameBuffer);

    deferred.LightsPass();
    deferred.FinalPass();

    renderGUI();
}

void SceneMgr::renderGUI()
{
    std::stringstream fps;
    fps << "FrameTime: " << std::setprecision(3) << BaseApp::frameTime << "ms" << " FPS: " << BaseApp::fps;

    int h = sConfig->GetDefault("height", WINDOW_HEIGHT);

    text2D.RenderText(fps.str(), 10, h - h*0.05, 12);

    if (sKeyboard->IsKeyPressed('L'))
    {
        text2D.RenderSprite(5, 5, 210, deferred.lightTexture);
        text2D.RenderSprite(5, 215, 210, deferred.colorTexture);
        text2D.RenderSprite(5, 425, 210, deferred.normalTexture);
    }
}

void SceneMgr::initLights()
{
    for (auto it = objects[TYPEID_COIN].begin(); it != objects[TYPEID_COIN].end(); ++it)
    {
        PointLight light;
        light.Position = it->second->GetPosition();
        light.Color = glm::vec3(1.0, 1.0, 0.0);
        light.Radius = 1.5f;
        light.Intensity = 2.0f;
        
        light.owner = it->second;
        lights.push_back(light);
    }
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
//     if (AABoundingBox* bounds = object->GetBoundingObject())
//         boundingBoxes[object->GetTypeId()].erase(bounds);

    unregisterQueue.push_back(object);
}
