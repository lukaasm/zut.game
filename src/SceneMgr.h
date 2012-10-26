#ifndef H_SCENEMGR
#define H_SCENEMGR

#include "Common.h"
#include "Singleton.h"

class Camera;
class RenderDevice;

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

};

#define sSceneMgr Singleton<SceneMgr>::Instance()
#endif