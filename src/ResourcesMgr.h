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
struct Vertex;

typedef std::hash_map<std::string, RenderData*> RenderDataMap;
typedef std::hash_map<std::string, uint32> TexturesMap;

class ResourcesMgr
{
    SINGLETON(ResourcesMgr)

    public:
        ~ResourcesMgr();

        void OnInit();

        RenderData* GetRenderDataForModel(std::string);
        uint32 GetTextureId(std::string);

    private:
        void loadTextures();
        void loadModels();
        void loadShaders() {}

        void unloadModels();
        void unloadTextures();

        //temp
        void genCube();
        void genSquare();
        uint32 createTexture(std::string);

        bool loadOBJ(std::string, std::vector<Vertex>&);

        RenderDataMap rendersData;
        TexturesMap textures;
        //uint32 GetTexture(std::string);
        //uint32 GetShader(std::string);
};

#define sResourcesMgr Singleton<ResourcesMgr>::Instance()
#endif
