#ifndef H_SCENEMGR
#define H_SCENEMGR

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common.h"
#include "DeferredRendering.h"
#include "Light.h"
#include "Singleton.h"
#include "Text2d.h"

class AABoundingBox;
class Camera;
class DynamicObject;
class GameObject;
class Shader;
class Terrain;

typedef std::unordered_map<uint32, GameObject*> GameObjectsMap;
typedef std::unordered_set<AABoundingBox*> BoundingBoxSet;
typedef std::list<PointLight> PointLightList;

class SceneMgr
{
    SINGLETON(SceneMgr)

    public:
        ~SceneMgr();

        void OnInit();
        void OnUpdate(const uint32 & diff);
        void OnRender();
        void OnResize(uint32 width, uint32 height);

        void RegisterObject(GameObject* object);
        void UnregisterObject(GameObject* object);

        void ToggleCamera();

        GameObjectsMap GetObjects();

        Camera* GetCamera();
        PointLightList& GetPointLights() { return lights; }
        DynamicObject* GetPlayer() { return player; }
        Terrain* GetTerrain() { return terrain; }
        GameObject* GetSkyBox() { return skybox; }

        float GetHeight(float, float, GameObject* = nullptr);
        float GetHeight(GameObject*);

        void CollisionTest(GameObject*);

    private:
        //void renderPass();
        void renderGUI();
        void initLights();

        uint32 guid;
        Text2D text2D;

        std::list<GameObject*> unregisterQueue;

        uint32 currentCamera;
        std::vector<Camera*> cameras;

        DynamicObject* player;
        Terrain* terrain;
        GameObject* skybox;

        GameObjectsMap staticObjects;
        GameObjectsMap dynamicObjects;

        BoundingBoxSet boundingBoxes;

        PointLightList lights;

        DeferredRenderer deferred;

};

#define sSceneMgr Singleton<SceneMgr>::Instance()
#endif
