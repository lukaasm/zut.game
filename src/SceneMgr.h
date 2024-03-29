#ifndef H_SCENEMGR
#define H_SCENEMGR

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common.h"
#include "DeferredRendering.h"
#include "GameObject.h"
#include "Light.h"
#include "Singleton.h"
#include "Text2d.h"

class AABoundingBox;
class Camera;
class DynamicObject;
class Shader;
class Terrain;

typedef std::unordered_map<uint32, GameObject*> GameObjectsMap;
typedef std::unordered_set<AABoundingBox*> BoundingBoxSet;
typedef std::list<PointLight> PointLightList;

enum GameState
{
    GAME_MENU            = 0,
    GAME_INPROGRESS      = 1,
    GAME_INPROGRESS_TUT1 = 2,
    GAME_END_SUCCESS     = 3,
    GAME_END_FAIL        = 4,
    GAME_END_EXIT        = 5
};

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

        //void RegisterBoundingBox(GameObject* object);

        void SetGameState(GameState s) { state = s; }

        void ToggleCamera();

        GameObjectsMap GetObjects();

        Camera* GetCamera();
        PointLightList& GetPointLights() { return lights; }
        DynamicObject* GetPlayer() { return player; }
        Terrain* GetTerrain() { return terrain; }
        GameObject* GetSkyBox() { return skybox; }

        float GetHeight(float, float, GameObject* = nullptr);
        float GetHeight(GameObject*);

        bool CollisionTest(GameObject* object, ObjectTypeId type);

    private:
        void renderGUI();
        void renderSkyBox();
        void initLights();
        void loadScene(std::string fileName);

        uint32 guid;
        Text2D text2D;

        uint32 state;

        uint32 monstersKilled;
        uint32 coinsOnMap;

        uint32 endTime;

        std::unordered_set<GameObject*> unregisterQueue;

        uint32 currentCamera;
        std::vector<Camera*> cameras;

        DynamicObject* player;
        Terrain* terrain;
        GameObject* skybox;

        GameObjectsMap objects[TYPEID_MAX];

        //BoundingBoxSet boundingBoxes[TYPEID_MAX];

        PointLightList lights;

        DeferredRenderer deferred;

};

#define sSceneMgr Singleton<SceneMgr>::Instance()
#endif
