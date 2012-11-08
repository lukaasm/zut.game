/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_RESOURCESMGR
#define H_RESOURCESMGR

#include <string>

#ifdef __GNUG__
#include <unordered_map>
#define hash_map unordered_map
#else
#include <hash_map>
#endif

#include <vector>

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

        uint32 createTexture(std::string);
        bool loadOBJ(std::string, std::vector<Vertex>&);

        void loadTexture(std::string fileName);
        bool loadModel(std::string fileName);

        RenderDataMap rendersData;
        TexturesMap textures;
        //uint32 GetTexture(std::string);
        //uint32 GetShader(std::string);
};

#define sResourcesMgr Singleton<ResourcesMgr>::Instance()
#endif
