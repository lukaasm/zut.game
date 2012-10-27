#ifndef H_SCENEMGR
#define H_SCENEMGR

#include <hash_map>

#include "Common.h"
#include "Singleton.h"

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
        Camera* camera;

        Shader* tempShader;

        GameObjectsMap gameObjectsMap;

};

#define sSceneMgr Singleton<SceneMgr>::Instance()
#endif