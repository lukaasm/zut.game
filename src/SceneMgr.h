#ifndef H_SCENEMGR
#define H_SCENEMGR

#ifdef __GNUG__
#include <unordered_map>
#define hash_map unordered_map
#else
#include <hash_map>
#endif

#include "Common.h"
#include "Singleton.h"
#include "Text2d.h"
#include "Timer.h"

class Camera;
class GameObject;
class Player;
class RenderDevice;
class Shader;

typedef std::hash_map<uint32, GameObject*> GameObjectsMap;

class SceneMgr
{
    SINGLETON(SceneMgr)

    public:
        ~SceneMgr();

        void OnInit();
        void OnUpdate(const uint32 & diff);
        void OnRender(RenderDevice*);
        void OnResize(uint32 width, uint32 height);

        void RegisterObject(GameObject* object);

        void ToggleCamera();

        Camera* GetCamera();
        Player* GetPlayer() { return player; }

        void CollisionTest(GameObject*);

    private:
        uint32 guid;
        Text2D text2D;

        uint32 currentCamera;
        std::vector<Camera*> cameras;

        Player* player;

        Shader* tempShader;

        Timer keyCheck;

        GameObjectsMap staticObjects;
        GameObjectsMap dynamicObjects;
};

#define sSceneMgr Singleton<SceneMgr>::Instance()
#endif
