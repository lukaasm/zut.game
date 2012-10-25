/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_RESOURCESMGR
#define H_RESOURCESMGR

#include <string>
#include <hash_map>

#include "Common.h"
#include "Singleton.h"

struct RenderData;
typedef std::hash_map<std::string, RenderData*> RenderDataMap;

class ResourcesMgr
{
    SINGLETON(ResourcesMgr)

    public:
        ~ResourcesMgr();

        void OnInit();

        RenderData* GetRenderDataForModel(std::string);

    private:
        void loadTextures() {}
        void loadModels();
        void loadShaders() {}

        void unloadModels();

        RenderDataMap rendersData;
        //uint32 GetTexture(std::string);
        //uint32 GetShader(std::string);
};

#define sResourcesMgr Singleton<ResourcesMgr>::Instance()
#endif