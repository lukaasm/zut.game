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

class Camera;
class GameObject;
class RenderDevice;
class Shader;

typedef std::hash_map<uint32, GameObject*> GameObjectsMap;

class SceneMgr
{
    SINGLETON(SceneMgr)

    public:
        ~SceneMgr();

        void OnInit();
        void OnUpdate(uint32 diff);
        void OnRender(RenderDevice*);

        Camera* GetCamera() { return camera; }
        void SetCamera(Camera*);

    private:
        Text2D text2D;

        Camera* camera;
        Shader* tempShader;

        GameObjectsMap gameObjectsMap;

};

#define sSceneMgr Singleton<SceneMgr>::Instance()
#endif
