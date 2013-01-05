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

ResourcesMgr::ResourcesMgr()
{
}

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
    std::cout << "[Model] loading file: " << fileName << std::endl;

    ModelData* md = ModelData::LoadModel("../res/models/" + fileName);
    if (md == nullptr)
    {
        std::string what = "[E][Model] problem with loading file: " + fileName;
        throw Exception(what);
    }

    modelsData[fileName] = md;
    return md;
}

uint32 ResourcesMgr::loadTexture(std::string fileName)
{
    std::cout << "[Texture] loading file: " << fileName << std::endl;

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

