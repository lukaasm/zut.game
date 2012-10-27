#include "ResourcesMgr.h"

#include <iostream>
#include <cstdio>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "GameObject.h"

void ResourcesMgr::OnInit()
{
    loadTextures();
    loadModels();
    loadTextures();
}

ResourcesMgr::~ResourcesMgr()
{
    unloadModels();
}

void ResourcesMgr::loadModels()
{
    std::string name = "grid";
    RenderData* renderData = new RenderData;

    uint32 _size = 40;
    Vertex vertices[160]; // _size*4

    for (uint32 x = 0; x < _size; x++)
    {
        vertices[x*2].position = vec3(float(x), 0.0f, 0.0f);
        vertices[x*2 +1].position = vec3(float(x), 0.0f, -float(_size));

        vertices[x*2 +_size*2].position = vec3(0.0f, 0.0f, -float(x));
        vertices[x*2 +_size*2+1].position = vec3(float(_size), 0.0f, -float(x));

        vertices[x*2].color = vec3(0.0f, 1.0f, 0.0f);
        vertices[x*2 +1].color = vec3(0.0f, 1.0f, 0.0f);

        vertices[x*2 +_size*2].color = vec3(0.0f, 1.0f, 0.0f);
        vertices[x*2 +_size*2+1].color = vec3(0.0f, 1.0f, 0.0f);
    };

    glGenVertexArrays(1, &(renderData->vertexArray));
    glBindVertexArray(renderData->vertexArray);
    {
        glGenBuffers(1, &(renderData->vertexBuffer));
        glBindBuffer(GL_ARRAY_BUFFER, renderData->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        {
            glVertexAttribPointer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            glVertexAttribPointer(VertexArray::Attrib::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(VertexArray::Attrib::POSITION);
        glEnableVertexAttribArray(VertexArray::Attrib::COLOR);
    }
    glBindVertexArray(0);

    renderData->size = 160;

    rendersData[name] = renderData;

    genCube();
    genSquare();
}

void ResourcesMgr::genCube()
{
    std::vector<Vertex> vertexes;
    if (!loadOBJ("../res/models/cube.obj", vertexes))
        return;

    RenderData* renderData = new RenderData();
    glGenVertexArrays(1, &(renderData->vertexArray));
    glBindVertexArray(renderData->vertexArray);
    {
        glGenBuffers(1, &(renderData->vertexBuffer));
        glBindBuffer(GL_ARRAY_BUFFER, renderData->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertexes.size()*sizeof(Vertex), &vertexes[0], GL_STATIC_DRAW);
        {
            glVertexAttribPointer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            glVertexAttribPointer(VertexArray::Attrib::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(VertexArray::Attrib::POSITION);
        glEnableVertexAttribArray(VertexArray::Attrib::TEXCOORD);
    }
    glBindVertexArray(0);

    renderData->size = vertexes.size();

    rendersData["cube"] = renderData;
}

void ResourcesMgr::genSquare()
{
    Vertex vert[6];
    for (uint8 i = 0; i < 6; ++i)
        vert[i].color = vec3(1.0f, 1.0f, 0.0f);

    vert[0].position = vec3(0.0f, 0.0f, 0.0f);
    vert[0].uv = vec2(0.0f, 0.0f);
    vert[1].position = vec3(1.0f, 0.0f, 0.0f);
    vert[1].uv = vec2(1.0f, 0.0f);
    vert[2].position = vec3(1.0f, 1.0f,0.0f);
    vert[2].uv = vec2(1.0f, 1.0f);

    vert[3].position = vec3(0.0f, 0.0f, 0.0f);
    vert[3].uv = vec2(0.0f, 0.0f);
    vert[4].position = vec3(0.0f, 1.0f, 0.0f);
    vert[4].uv = vec2(0.0f, 1.0f);
    vert[5].position = vec3(1.0f, 1.0f, 0.0f);
    vert[5].uv = vec2(1.0f, 1.0f);

    RenderData* renderData = new RenderData();
    glGenVertexArrays(1, &(renderData->vertexArray));
    glBindVertexArray(renderData->vertexArray);
    {
        glGenBuffers(1, &(renderData->vertexBuffer));
        glBindBuffer(GL_ARRAY_BUFFER, renderData->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
        {
            glVertexAttribPointer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            glVertexAttribPointer(VertexArray::Attrib::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(VertexArray::Attrib::POSITION);
        glEnableVertexAttribArray(VertexArray::Attrib::TEXCOORD);
    }
    glBindVertexArray(0);

    renderData->size = 6;

    rendersData["square"] = renderData;
}

void ResourcesMgr::unloadModels()
{
    for (RenderDataMap::iterator i = rendersData.begin(); i != rendersData.end(); ++i)
        delete i->second;

    rendersData.clear();
}

RenderData* ResourcesMgr::GetRenderDataForModel(std::string name)
{
    if (rendersData.find(name.c_str()) != rendersData.end())
        return rendersData[name.c_str()];

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

bool ResourcesMgr::loadOBJ(std::string fileName, std::vector<Vertex>& vert)
{
    FILE* file = fopen(fileName.c_str(), "r");
    if (file == NULL)
        return false;

    std::vector<vec3> temp_vertices, temp_normals;
    std::vector<vec2> temp_uvs;

    std::vector<uint32> vertIndices, uvIndices, normIndices;
    while (1)
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0)
        {
            vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            vec3 normal;
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

            vertIndices.push_back(vertexIndex[0]);
            vertIndices.push_back(vertexIndex[1]);
            vertIndices.push_back(vertexIndex[2]);

            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);

            normIndices.push_back(normalIndex[0]);
            normIndices.push_back(normalIndex[1]);
            normIndices.push_back(normalIndex[2]);
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
        uint32 vertIndex = vertIndices[i];
        uint32 uvIndex = uvIndices[i];
        uint32 normIndex = normIndices[i];

        Vertex vertex;
        vertex.position = temp_vertices[vertIndex-1];
        vertex.uv = temp_uvs[uvIndex-1];
        vertex.normal = temp_normals[normIndex-1];

        vert.push_back(vertex);
    }

    return true;
}

void ResourcesMgr::loadTextures()
{
    uint32 textureId = createTexture("../res/textures/test.tga");
    if (textureId == 0)
    {
        std::cout << "There is no such file." << std::endl;
        return;
    }

    textures["test"] = textureId;
}
