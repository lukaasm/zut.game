/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_RESOURCESMGR
#define H_RESOURCESMGR

#include <string>

#include <unordered_map>
#include <vector>

#include "Common.h"
#include "Singleton.h"

struct Vertex;
struct ModelData;

typedef std::unordered_map<std::string, ModelData*> ModelDataMap;
typedef std::unordered_map<std::string, uint32> TexturesMap;

class ResourcesMgr
{
    SINGLETON(ResourcesMgr)

    public:
        ~ResourcesMgr();

        void OnInit();

        ModelData* GetModelData(std::string);
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

        ModelDataMap modelsData;
        TexturesMap textures;
        //uint32 GetTexture(std::string);
        //uint32 GetShader(std::string);
};

#define sResourcesMgr Singleton<ResourcesMgr>::Instance()
#endif
