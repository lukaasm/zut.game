#ifndef H_SCENEMGR
#define H_SCENEMGR

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common.h"
#include "Singleton.h"
#include "Text2d.h"

class AABoundingBox;
class Camera;
class GameObject;
class Player;
class RenderDevice;
class Shader;
class Terrain;

typedef std::unordered_map<uint32, GameObject*> GameObjectsMap;
typedef std::unordered_set<AABoundingBox*> BoundingBoxSet;

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

        float GetHeight(float, float);
        void CollisionTest(GameObject*);

    private:
        uint32 guid;
        Text2D text2D;

        uint32 currentCamera;
        std::vector<Camera*> cameras;

        Player* player;
        Terrain* terrain;

        GameObjectsMap staticObjects;
        GameObjectsMap dynamicObjects;
        BoundingBoxSet boundingBoxes;

};

#define sSceneMgr Singleton<SceneMgr>::Instance()
#endif
