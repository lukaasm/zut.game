#include "ResourcesMgr.h"

#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstring>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "BoundingObject.h"
#include "Exception.h"
#include "GameObject.h"
#include "ModelData.h"
#include "Shader.h"
#include "VertexArrayObject.h"

void ResourcesMgr::OnInit()
{
    loadShaders();
}

ResourcesMgr::~ResourcesMgr()
{
    unloadModels();
    unloadTextures();
}

void ResourcesMgr::unloadModels()
{
    std::for_each(modelsData.begin(), modelsData.end(), [] (std::pair<std::string, ModelData*> p) { delete p.second; });
    modelsData.clear();
}

ModelData* ResourcesMgr::GetModelData(std::string name)
{
    if (modelsData.find(name) != modelsData.end())
        return modelsData[name];
    else
        return loadModel(name);
}

uint32 ResourcesMgr::GetTextureId(std::string name)
{
    if (textures.find(name) != textures.end())
        return textures[name];
    else
        return loadTexture(name);
}

uint32 ResourcesMgr::createTexture(std::string fileName)
{
    if (fileName.find(".tga") == std::string::npos)
        return 0;

    uint32 textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    if (glfwLoadTexture2D(fileName.c_str(), GLFW_BUILD_MIPMAPS_BIT))
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        return textureId;
    }

    return 0;
}

ModelData* ResourcesMgr::loadModel(std::string fileName)
{
    std::cout << std::endl << "[Model] loading file: " << fileName;

    /*if (GetModelData(fileName) != nullptr)
    {
        std::string what = "[E][Model] file: " + fileName + " were already loaded.";
        throw Exception(what);
    }*/

    std::vector<Vertex> vertexes;
    if (!loadOBJ("../res/models/" + fileName, vertexes))
    {
        std::string what = "[E][Model] problem with loading file: " + fileName;
        throw Exception(what);
    }

    ModelData* modelData = new ModelData();
    VertexArrayObject& vao = modelData->vao;
    vao.CreateVertexArray();
    vao.CreateVertexBuffer();

    vao.Bind(ID_VAO);
    vao.Bind(ID_VBO);
    vao.FillBuffer(GL_STATIC_DRAW, &vertexes[0], vertexes.size()*sizeof(Vertex));
    vao.EnableAttrib(VertexArray::Attrib::POSITION);
    vao.AddAttribToBuffer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    vao.EnableAttrib(VertexArray::Attrib::NORMAL);
    vao.AddAttribToBuffer(VertexArray::Attrib::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(NORMAL_VERTEX_POS));
    vao.EnableAttrib(VertexArray::Attrib::COLOR);
    vao.AddAttribToBuffer(VertexArray::Attrib::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(COLOR_VERTEX_POS));
    vao.EnableAttrib(VertexArray::Attrib::TEXCOORD);
    vao.AddAttribToBuffer(VertexArray::Attrib::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(UV_VERTEX_POS));

    vao.Unbind(ID_VBO);
    vao.Unbind(ID_VAO);

    vao.ElementsCount() = vertexes.size();

    BoundingBoxProto* box = new BoundingBoxProto();
    box->SetMinMax(vertexes);

    modelData->height = box->GetMax().z - box->GetMin().z;
    modelData->boundingBox = box;

    modelsData[fileName] = modelData;
    return modelData;
}

bool ResourcesMgr::loadOBJ(std::string fileName, std::vector<Vertex>& vert)
{
    if (fileName.find(".obj") == std::string::npos)
        return false;

    FILE* file = fopen(fileName.c_str(), "r");
    if (file == NULL)
        return false;

    std::vector<glm::vec3> temp_vert, temp_normals;
    std::vector<glm::vec2> temp_uvs;

    std::vector<uint32> vertIndices, uvIndices, normIndices;
    while (1)
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vert.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            uint32 vertexIndex[3], uvIndex[3], normalIndex[3];
            if (fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]) != 9)
            {
                std::cout << "[E][Model] can NOT load OBJ file: " << fileName << " weird line format for 'f'" << std::endl;
                return false;
            }

            vertIndices.push_back(--vertexIndex[0]);
            vertIndices.push_back(--vertexIndex[1]);
            vertIndices.push_back(--vertexIndex[2]);

            uvIndices.push_back(--uvIndex[0]);
            uvIndices.push_back(--uvIndex[1]);
            uvIndices.push_back(--uvIndex[2]);

            normIndices.push_back(--normalIndex[0]);
            normIndices.push_back(--normalIndex[1]);
            normIndices.push_back(--normalIndex[2]);
        }
        else
        {
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    vert.clear();

    for (uint32 i = 0; i < vertIndices.size(); ++i)
    {
        Vertex vertex;
        vertex.position = temp_vert[vertIndices[i]];
        vertex.uv = temp_uvs[uvIndices[i]];
        vertex.normal = temp_normals[normIndices[i]];

        vert.push_back(vertex);
    }

    return true;
}

uint32 ResourcesMgr::loadTexture(std::string fileName)
{
    std::cout << std::endl << "[Texture] loading file: " << fileName;
//     if (GetTextureId(fileName))
//     {
//         std::string what = "[E][Texture] file: " + fileName + " were already loaded.";
//         throw Exception(what);
//     }

    uint32 textureId = createTexture("../res/textures/" + fileName);
    if (textureId == 0)
    {
        std::string what = "[E][Texture] problem occurred while creating texture from file: " + fileName;
        throw Exception(what);
    }

    textures[fileName] = textureId;
    return textureId;
}

void ResourcesMgr::unloadTextures()
{
    std::for_each(textures.begin(), textures.end(), [](std::pair<std::string, uint32> p) { glDeleteTextures(1, &(p.second)); });
    textures.clear();
}

void ResourcesMgr::unloadShaders()
{
    std::for_each(shaders.begin(), shaders.end(), [](std::pair<std::string, Shader*> p) { delete p.second;});
    shaders.clear();
}

Shader* ResourcesMgr::GetShader(std::string key)
{
    if (shaders.find(key) != shaders.end())
        return shaders[key];

    return nullptr;
}

void ResourcesMgr::loadShaders()
{
    Shader* shader = (new Shader())->LoadFromFile("../res/shaders/text2d.glsl");

    shader->AddUniform("textureSampler");
    shader->AddUniform("in_ScreenSize");

    shader->AddAttribute(VertexArray::Attrib::POSITION, "in_Position");
    shader->AddAttribute(VertexArray::Attrib::TEXCOORD, "in_TexCoord");
    shaders["text2d.glsl"] = shader;

    shader = (new Shader())->LoadFromFile("../res/shaders/simple.glsl");

    shader->AddUniform("in_MVP");

    shader->AddAttribute(VertexArray::Attrib::POSITION, "in_Position");
    shader->AddAttribute(VertexArray::Attrib::TEXCOORD, "in_TexCoord");
    shaders["simple.glsl"] = shader;

    shader = (new Shader())->LoadFromFile("../res/shaders/deferred_geopass.glsl");

    shader->AddUniform("in_MVP");
    shader->AddUniform("in_M");

    shader->AddUniform("Texture");

    shader->AddAttribute(VertexArray::Attrib::POSITION, "in_Position");
    shader->AddAttribute(VertexArray::Attrib::TEXCOORD, "in_TexCoord");
    shader->AddAttribute(VertexArray::Attrib::NORMAL, "in_Normal");
    shaders["deferred_geopass.glsl"] = shader;

    shader = (new Shader())->LoadFromFile("../res/shaders/deferred_dirlightpass.glsl");

    shader->AddUniform("in_CameraPosition");
    shader->AddUniform("in_InvVP");

    shader->AddUniform("ColorTexture");
    shader->AddUniform("NormalTexture");
    shader->AddUniform("DepthTexture");

    shader->AddDirectionalLight();

    shader->AddAttribute(VertexArray::Attrib::POSITION, "in_Position");
    shader->AddAttribute(VertexArray::Attrib::TEXCOORD, "in_TexCoord");

    shaders["deferred_dirlightpass.glsl"] = shader;

    shader = (new Shader())->LoadFromFile("../res/shaders/deferred_pointlightpass.glsl");

    shader->AddUniform("in_MVP");
    shader->AddUniform("in_CameraPosition");
    shader->AddUniform("in_InvVP");

    shader->AddUniform("ColorTexture");
    shader->AddUniform("NormalTexture");
    shader->AddUniform("DepthTexture");

    shader->AddPointLight();

    shader->AddAttribute(VertexArray::Attrib::POSITION, "in_Position");

    shaders["deferred_pointlightpass.glsl"] = shader;

    shader = (new Shader())->LoadFromFile("../res/shaders/deferred_finalpass.glsl");

    shader->AddUniform("ColorTexture");
    shader->AddUniform("LightTexture");

    shader->AddAttribute(VertexArray::Attrib::POSITION, "in_Position");
    shader->AddAttribute(VertexArray::Attrib::TEXCOORD, "in_TexCoord");

    shaders["deferred_finalpass.glsl"] = shader;
}
