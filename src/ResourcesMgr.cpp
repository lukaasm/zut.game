#include "ResourcesMgr.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <exception>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "BoundingObject.h"
#include "GameObject.h"
#include "ModelData.h"
#include "VertexArrayObject.h"

// warning C4482: nonstandard extension used: enum 'VertexArray::Attrib' used in qualified name
#pragma warning(disable : 4482)
// warning C4996: 'fscanf': This function or variable may be unsafe. Consider using fscanf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable : 4996)

class Exception: public std::exception
{
    public:
        Exception(std::string & whatStr) : std::exception()
        {
            m_whatStr = whatStr;
        }

        virtual const char* what() const throw()
        {
            return m_whatStr.c_str();
        }

    private:
        std::string m_whatStr;
};

void ResourcesMgr::OnInit()
{
    loadTextures();
    loadModels();
}

ResourcesMgr::~ResourcesMgr()
{
    unloadModels();
    unloadTextures();
}

void ResourcesMgr::loadModels()
{
    std::string name = "grid";
    ModelData* modelData = new ModelData();

    uint32 _size = 40;
    Vertex vert[160]; // _size*4

    for (uint32 i = 0; i < 160; ++i)
        vert[i].color = glm::vec3(0.5f, 0.0f, 0.3f);

    for (uint32 x = 0; x < _size; x++)
    {
        vert[x*2].position = glm::vec3(float(x), 0.0f, 0.0f);
        vert[x*2 +1].position = glm::vec3(float(x), 0.0f, -float(_size));

        vert[x*2 +_size*2].position = glm::vec3(0.0f, 0.0f, -float(x));
        vert[x*2 +_size*2+1].position = glm::vec3(float(_size), 0.0f, -float(x));
    };

    VertexArrayObject& vao = modelData->vao;

    vao.CreateVertexArray();
    vao.CreateVertexBuffer();

    vao.Bind(ID_VAO);
    vao.Bind(ID_VBO);

    vao.FillBuffer(GL_STATIC_DRAW, vert, sizeof(vert));

    vao.EnableAttrib(VertexArray::Attrib::POSITION);
    vao.AddAttribToBuffer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    vao.EnableAttrib(VertexArray::Attrib::COLOR);
    vao.AddAttribToBuffer(VertexArray::Attrib::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(COLOR_VERTEX_POS));

    vao.Unbind(ID_VBO);
    vao.Unbind(ID_VAO);

    vao.ElementsCount() = 160;

    modelsData[name] = modelData;

    loadModel("cube.obj");
    loadModel("trashbin.obj");
}

void ResourcesMgr::unloadModels()
{
    for (ModelDataMap::iterator i = modelsData.begin(); i != modelsData.end(); ++i)
        delete i->second;

    modelsData.clear();
}

ModelData* ResourcesMgr::GetModelData(std::string name)
{
    if (modelsData.find(name.c_str()) != modelsData.end())
        return modelsData[name.c_str()];

    return nullptr;
}

uint32 ResourcesMgr::GetTextureId(std::string name)
{
    if (textures.find(name.c_str()) != textures.end())
        return textures[name.c_str()];

    return 0;
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        return textureId;
    }
    return 0;
}

bool ResourcesMgr::loadModel(std::string fileName)
{
    try
    {
        if (GetModelData(fileName) != nullptr)
        {
            std::string what = "Error: file: " + fileName + " were already loaded.";
            throw Exception(what);
        }

        std::vector<Vertex> vertexes;
        if (!loadOBJ("../res/models/" + fileName, vertexes))
        {
            std::string what = "Error: problem with loading file: " + fileName;
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
        vao.EnableAttrib(VertexArray::Attrib::TEXCOORD);
        vao.AddAttribToBuffer(VertexArray::Attrib::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(UV_VERTEX_POS));

        vao.Unbind(ID_VBO);
        vao.Unbind(ID_VAO);

        vao.ElementsCount() = vertexes.size();

        BoundingBox* box = new BoundingBox();
        box->SetMinMax(vertexes);

        modelData->boundingObject = box;

        modelsData[fileName] = modelData;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return true;
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
                std::cout << "ERROR: can NOT load OBJ file: " << fileName << " weird line format for 'f'" << std::endl;
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

void ResourcesMgr::loadTexture(std::string fileName)
{
    try
    {
        if (GetTextureId(fileName))
        {
            std::string what = "Error: file: " + fileName + " were already loaded.";
            throw Exception(what);
        }

        uint32 textureId = createTexture("../res/textures/" + fileName);
        if (textureId == 0)
        {
            std::string what = "Error: problem occurred while creating texture from file: " + fileName;
            throw Exception(what);
        }

        textures[fileName] = textureId;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        // exit program
    }
}

void ResourcesMgr::loadTextures()
{
    loadTexture("test.tga");
    loadTexture("cube.tga");
    loadTexture("font.tga");
}

void ResourcesMgr::unloadTextures()
{
    for (TexturesMap::iterator i = textures.begin(); i != textures.end(); ++i)
        glDeleteTextures(1, &(i->second));

    textures.clear();
}
