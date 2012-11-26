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
class Shader;

typedef std::unordered_map<std::string, ModelData*> ModelDataMap;
typedef std::unordered_map<std::string, uint32> TexturesMap;
typedef std::unordered_map<std::string, Shader*> ShadersMap;

class ResourcesMgr
{
    SINGLETON(ResourcesMgr)

    public:
        ~ResourcesMgr();

        void OnInit();

        ModelData* GetModelData(std::string);
        uint32 GetTextureId(std::string);
        Shader* GetShader(std::string);

        void loadShaders();

    private:
        void loadTextures();
        void loadModels();
       // void loadShaders();

        void unloadModels();
        void unloadTextures();
        void unloadShaders();

        uint32 createTexture(std::string);
        bool loadOBJ(std::string, std::vector<Vertex>&);

        void loadTexture(std::string fileName);
        bool loadModel(std::string fileName);

        ModelDataMap modelsData;
        TexturesMap textures;
        ShadersMap shaders;
};

#define sResourcesMgr Singleton<ResourcesMgr>::Instance()
#endif
