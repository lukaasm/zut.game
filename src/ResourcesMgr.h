/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_RESOURCESMGR
#define H_RESOURCESMGR

#include "Common.h"
#include "Singleton.h"

class ResourcesMgr
{
    INIT_SINGLETON(ResourcesMgr)

    public:
        void OnInit();

    private:
        void loadTextures() {}
        void loadModels() {}
        void loadShaders() {}

        //uint32 GetTexture(std::string);
        //uint32 GetModel(std::string);
        //uint32 GetShader(std::string);
};

#define sResourcesMgr Singleton<ResourcesMgr>::Instance()
#endif