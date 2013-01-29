#include "SceneMgr.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "BaseApp.h"
#include "BoundingObject.h"
#include "Camera.h"
#include "Config.h"
#include "DynamicObject.h"
#include "Exception.h"
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
    state = GAME_MENU;
}

void SceneMgr::loadScene(std::string fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
        throw Exception("[Scene][E] cannot open file that contains scene information");

    GameObject* ob = nullptr;

    char type;
    while (!file.eof())
    {
        file.ignore(100, ';');
        file >> type;

        float x, z;
        file >> x >> z;

        switch (type)
        {
            case 'g': ADDGRASS(x, z) break;
            case 's': ADDSHROOMS(x, z) break;
            case 'r': ADDROCK(x, z) break;
            case 'e': ADDENEMY(x, z) break;
            case 'c': ADDCOIN(x, z) break;
            case 'p':
            case '1':
            case '2':
            {
                float yOffset;
                file >> yOffset;

                if (type == 'p')
                {
                    ADDPALM(x, z, yOffset)
                }
                else if (type == '1')
                {
                    ADDSTATICOBJECT(true, "wall.obj", "wall.tga", x, z, yOffset)
                    //if (sRandom->Int(0, 1))
                    //    ob->SetOrientation(180.0f);
                }
                else
                {
                    ADDSTATICOBJECT(true, "wall_r.obj", "wall.tga", x, z, yOffset)
                    //if (sRandom->Int(0, 1))
                    //    ob->SetOrientation(180.0f);

                    if (yOffset < 0.0f)
                    {
                        glm::vec3 s = ob->GetScale();
                        s.y = 0.6f;
                        ob->SetScale(s);
                    }
                }
                break;
            }
        }
    }
}

void SceneMgr::OnInit()
{
    guid = 0;
    monstersKilled = 0;

    terrain = new Terrain();

    player = new DynamicObject("mage.obj", "mage.tga");

    player->SETPOSITION2(17.06f, 8.6f, -0.20f, player);
    player->SetScale(glm::vec3(0.075f));
    player->EnableBoundingBox();
    player->scripts["OnUpdate"].push_back(
        [](DynamicObject& ob)
        {
            if (sMouse->IsButtonPressed(GLFW_MOUSE_BUTTON_1))
            {   
                uint32 x = sMouse->GetPos().x;
                uint32 centerX = sConfig->GetDefault("width", WINDOW_WIDTH) * 0.5f;
                float sens = sConfig->GetDefault("sensitivity", 1.0f);
                int32 deltax = centerX - x;
                ob.SetOrientation(ob.GetOrientation() + deltax*0.3f*sens);

                sMouse->SetPos(centerX, sMouse->GetPos().y);
                ob.AddMoveType(moveInfos[MOVE_TYPE_FORWARD]);
            }
            else
                ob.ClearMoveType(MOVE_FLAG_FORWARD);

            sSceneMgr->CollisionTest(&ob, TYPEID_COIN);
        });

    player->timers[2].Start(10000);

    player->scripts["OnUpdate"].push_back(
        [](DynamicObject& ob)
        {
            if (ob.timers[4].Passed())
                ob.modelName = "mage.obj";

            if (!sMouse->IsButtonPressed(GLFW_MOUSE_BUTTON_2))
                return;
            
            ob.timers[4].Start(500);
            ob.modelName = "mage2.obj";

            sMouse->OnButtonRelease(GLFW_MOUSE_BUTTON_2);

            DynamicObject* shoot = new DynamicObject("fireball.obj", "fireball.tga");
            shoot->SetTypeId(TYPEID_PROJECTILE);
            shoot->SetPosition(ob.GetPosition());
            shoot->SetScale(glm::vec3(0.025f));
            shoot->AddMoveType(moveInfos[MOVE_TYPE_FORWARD]);
            shoot->SetOrientation(sSceneMgr->GetPlayer()->GetOrientation());
            shoot->EnableBoundingBox();

            shoot->scripts["OnUpdate"].push_back(
                [](DynamicObject& ob)
                {
                    ob.SetPosition(Position(ob.GetPosition().x, sSceneMgr->GetHeight(&ob) + 0.25f, ob.GetPosition().z));

                    // life span
                    if (ob.timers[0].Elapsed() > 3000)
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
            light.Color = glm::vec3(0.8f, 0.4f, 0.0f);
            light.Intensity = 1.0f;
            light.Radius = 8.0f;

            sSceneMgr->GetPointLights().push_back(light);
            sSceneMgr->RegisterObject(shoot);
        });

    player->scripts["OnUpdate"].push_back(
        [](DynamicObject& ob)
        {
            if (!sMouse->IsButtonPressed(GLFW_MOUSE_BUTTON_3) && !sKeyboard->IsKeyPressed(GLFW_KEY_SPACE))
                return;

            if (!ob.timers[2].Passed())
                return;

            ob.timers[2].Start(10000);

            sMouse->OnButtonRelease(GLFW_MOUSE_BUTTON_3);
            sKeyboard->OnKeyRelease(GLFW_KEY_SPACE);

            DynamicObject* explo = new DynamicObject("disc.obj", "disc.tga");
            explo->SetTypeId(TYPEID_PROJECTILE);
            Position pos = ob.GetPosition();
            pos.y += 0.1f;
            explo->SetPosition(pos);
            explo->SetScale(glm::vec3(0.15f));
            explo->AddMoveType(moveInfos[MOVE_TYPE_ROTATE_RIGHT]);

            explo->EnableBoundingBox();

            explo->scripts["OnUpdate"].push_back(
                [](DynamicObject& ob)
                {
                    float v = ob.timers[0].Elapsed()*0.0030f;
                    ob.SetScale(glm::vec3(v, 2.0f, v));
                    // life span
                    if (ob.timers[0].Elapsed() > 500)
                    {
                         sSceneMgr->UnregisterObject(&ob);
                         return;
                    }
                    sSceneMgr->CollisionTest(&ob, TYPEID_DYNAMIC);
                });

            PointLight light;
            light.owner = explo;
            light.Color = glm::vec3(0.8f, 0.4f, 0.0f);
            light.Intensity = 1.0f;
            light.Radius = 3.0f;

            sSceneMgr->GetPointLights().push_back(light);
            sSceneMgr->RegisterObject(explo);
        });

    RegisterObject(player);

    cameras.push_back(new TppCamera());
    cameras.push_back(new FppCamera());
    cameras.push_back(new EagleEyeCamera());

    std::for_each(cameras.begin(), cameras.end(), [this](Camera* c) { c->LinkTo(player); });

    currentCamera = 0;

    skybox = new GameObject("cube.obj", "skybox.tga");
    skybox->SetPosition(Position(0,0,0));
    skybox->SetScale(glm::vec3(100));

    // LEN :P
    GameObject* ob;
    ADDENEMY(17.1723f, 11.6584f)

    loadScene("../res/scene");

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

    ADDSTATICOBJECT(true, "border.obj", "border.tga", 36.5159, 0.55867, 1.25f)
    ob->SetScale(glm::vec3(1.6f, 0.8f, 0.7f));

    ADDSTATICOBJECT(true, "border.obj", "border.tga", 36.5159, 61.5892, 1.25f)
    ob->SetScale(glm::vec3(1.6f, 0.8f, 0.7f));
    
    ADDSTATICOBJECT(true, "border_r.obj", "border.tga", 1.25867, 36.5159, -0.75f)
    ob->SetScale(glm::vec3(0.7f, 0.8f, 1.6f));

    ADDSTATICOBJECT(true, "border_r.obj", "border.tga",  62.8892, 36.5159, 1.25f)
    ob->SetScale(glm::vec3(0.7f, 0.8f, 1.6f));

    coinsOnMap = objects[TYPEID_COIN].size() - 1;

    endTime = 0;

    text2D.Init();

    initLights();
    deferred.Init();
}

void SceneMgr::OnUpdate(const uint32& diff)
{
    GetCamera()->OnUpdate(diff);

    if (sKeyboard->IsKeyPressed('X'))
    {
        if (state == GAME_MENU)
        {
            state = GAME_INPROGRESS;
            sKeyboard->OnKeyRelease('X');
        }
        else
            state = GAME_END_EXIT;
    }

    if (state != GAME_INPROGRESS)
        return;

    if (player->GetHealth() <= 0)
    {
        state = GAME_END_FAIL;
        return;
    }

    for (uint8 i = TYPEID_DYNAMIC; i < TYPEID_MAX; ++i)
    {
        for (auto it = objects[i].begin(); it != objects[i].end(); ++it)
            reinterpret_cast<DynamicObject*>(it->second)->OnUpdate(diff);
    }

    float time = glfwGetTime()*150.0f;
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
            else if (i->owner->GetTypeId() == TYPEID_PROJECTILE && i->owner->GetModel() == "disc.obj")
            {
                i->Position.y += 1.2f;
                i->Radius = 5.5f * i->owner->GetScale().x;
            }
        }

        ++i;
    }

    while (!unregisterQueue.empty())
    {
        auto i = unregisterQueue.begin();

        GameObject* object = *i;
        objects[object->GetTypeId()].erase(object->GetGuid());

        delete object;
        unregisterQueue.erase(i);
    }
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
    deferred.ShadowPass();
    deferred.GeometryPass();
    deferred.LightsPass();
    deferred.FinalPass();

    renderGUI();
}

void SceneMgr::renderGUI()
{
    std::stringstream fps;
    fps << "FrameTime: " << std::setprecision(3) << BaseApp::frameTime << "ms" << " FPS: " << BaseApp::fps;

    int h = sConfig->GetDefault("height", WINDOW_HEIGHT);
    int w = sConfig->GetDefault("width", WINDOW_WIDTH);

    text2D.RenderText(fps.str(), 10, h - h*0.05, 12);

    switch (state)
    {
        case GAME_MENU:
        {
            uint32 instId = sResourcesMgr->GetTextureId("gui_begin.tga");
            text2D.RenderSprite(w/2 - 400, h/2 - 400, 800, instId);
            break;
        }
        case GAME_INPROGRESS:
        {
            if (!(player->timers[3].Passed()))
            {
                uint32 clawId = sResourcesMgr->GetTextureId("clawmark.tga");
                text2D.RenderSprite(w/2 - 600, h/2 - 600, 1200, clawId);
            }

            std::stringstream hp;
            hp << "Health: " << player->GetHealth();
            text2D.RenderText(hp.str(), w/2 - hp.str().length()*0.5f*18, 30, 18);

            std::stringstream kills;
            kills << monstersKilled;
            text2D.RenderText(kills.str(), 275, 32, 16);
            uint32 kill = sResourcesMgr->GetTextureId("gui_kill.tga");
            text2D.RenderSprite(205, 15, 50, kill);

            uint32 coin = sResourcesMgr->GetTextureId("gui_coin.tga");
            text2D.RenderSprite(15, 15, 50, coin);

            std::stringstream points;
            uint32 coinsCollected = coinsOnMap - (objects[TYPEID_COIN].size()-1);
            points << coinsCollected << "/" << coinsOnMap;
            text2D.RenderText(points.str(), 80, 32, 16);

            if (coinsOnMap == coinsCollected)
            {
                state = GAME_END_SUCCESS;
                endTime = player->timers[0].Elapsed() * 0.001f;
            }

            std::stringstream t;
            t << "Press X to exit game";
            text2D.RenderText(t.str(), w/2 - t.str().length()*0.5f*15, h - h*0.05, 15);
// 
            int cooldown = 10 - int(player->timers[2].Elapsed()*0.001f);
            if (cooldown > 0)
            {
                uint32 gui = sResourcesMgr->GetTextureId("gui_cooldown.tga");
                text2D.RenderSprite(w-225, 0, 225, gui);

                std::stringstream cd;
                cd << cooldown << "s";
                text2D.RenderText(cd.str(), w-125, 82, 32);
            }

            if (sKeyboard->IsKeyPressed('L'))
            {
                text2D.RenderSprite(5, 0, 200, deferred.colorTexture);
                text2D.RenderSprite(5, 205, 200, deferred.normalTexture);
                text2D.RenderSprite(5, 410, 200, deferred.depthTexture);
            }
            break;
        }
        case GAME_INPROGRESS_TUT1:
        {
            uint32 gui = sResourcesMgr->GetTextureId("gui_tutorial1.tga");
            text2D.RenderSprite(w/2 - 300, h/2 - 300, 600, gui);

            if (sMouse->IsButtonPressed(GLFW_MOUSE_BUTTON_2))
                state = GAME_INPROGRESS;

            break;
        }
        case GAME_END_FAIL:
        {
            uint32 instId = sResourcesMgr->GetTextureId("gui_end1.tga");
            text2D.RenderSprite(w/2 - 400, h/2 - 400, 800, instId);
            break;
        }
        case GAME_END_SUCCESS:
        {
            uint32 instId = sResourcesMgr->GetTextureId("gui_end2.tga");
            text2D.RenderSprite(w/2 - 400, h/2 - 400, 800, instId);

            std::stringstream time;
            time << std::setprecision(2) << ((endTime / 60) % 60) << "m " << (endTime % 60) <<"s";
            text2D.RenderText(time.str(), w/2 - 20, h/2 - 250 + 220, 35);
            
            std::stringstream kills;
            kills << monstersKilled;
            text2D.RenderText(kills.str(), w/2 - 20, h/2 - 250 + 190, 35);
            break;
        }
        case GAME_END_EXIT:
        {
            BaseApp::CloseWindow();
            break;
        }
    }
}

void SceneMgr::initLights()
{
    for (auto it = objects[TYPEID_COIN].begin(); it != objects[TYPEID_COIN].end(); ++it)
    {
        PointLight light;
        light.Position = it->second->GetPosition();
        light.Color = glm::vec3(1.0, 1.0, 0.0);
        light.Radius = 0.95f;
        light.Intensity = 1.0f;
        
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
    if (object->GetModel() == "boid.obj")
        ++monstersKilled;

    unregisterQueue.insert(object);
}
