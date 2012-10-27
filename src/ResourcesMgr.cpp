#include "ResourcesMgr.h"

#include <iostream>
#include <GL/glew.h>
#include <gli/gli.hpp>
#include <gli/gtx/gl_texture2d.hpp>

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
    Vertex vert[6*6];
    for (uint8 i = 0; i < 36; ++i)
        vert[i].color = vec3(1.0f, 1.0f, 0.0f);

    vert[0].position = vec3(-1.0f,-1.0f,-1.0f);
    vert[1].position = vec3(-1.0f,-1.0f, 1.0f);
    vert[2].position = vec3(-1.0f, 1.0f, 1.0f);
    vert[3].position = vec3(1.0f, 1.0f,-1.0f);
    vert[4].position = vec3(-1.0f,-1.0f,-1.0f);
    vert[5].position = vec3(-1.0f, 1.0f,-1.0f);
    vert[6].position = vec3(1.0f,-1.0f, 1.0f);
    vert[7].position = vec3(-1.0f,-1.0f,-1.0f);
    vert[8].position = vec3(1.0f,-1.0f,-1.0f);
    vert[9].position = vec3(1.0f, 1.0f,-1.0f);
    vert[10].position = vec3(1.0f,-1.0f,-1.0f);
    vert[11].position = vec3(-1.0f,-1.0f,-1.0f);
    vert[12].position = vec3(-1.0f,-1.0f,-1.0f);
    vert[13].position = vec3(-1.0f, 1.0f, 1.0f);
    vert[14].position = vec3(-1.0f, 1.0f,-1.0f);
    vert[15].position = vec3(1.0f,-1.0f, 1.0f);
    vert[16].position = vec3(-1.0f,-1.0f, 1.0f);
    vert[17].position = vec3(-1.0f,-1.0f,-1.0f);
    vert[18].position = vec3(-1.0f, 1.0f, 1.0f);
    vert[19].position = vec3(-1.0f,-1.0f, 1.0f);
    vert[20].position = vec3(1.0f,-1.0f, 1.0f);
    vert[21].position = vec3(1.0f, 1.0f, 1.0f);
    vert[22].position = vec3(1.0f,-1.0f,-1.0f);
    vert[23].position = vec3(1.0f, 1.0f,-1.0f);
    vert[24].position = vec3(1.0f,-1.0f,-1.0f);
    vert[25].position = vec3(1.0f, 1.0f, 1.0f);
    vert[26].position = vec3(1.0f,-1.0f, 1.0f);
    vert[27].position = vec3(1.0f, 1.0f, 1.0f);
    vert[28].position = vec3(1.0f, 1.0f,-1.0f);
    vert[29].position = vec3(-1.0f, 1.0f,-1.0f);
    vert[30].position = vec3(1.0f, 1.0f, 1.0f);
    vert[31].position = vec3(-1.0f, 1.0f,-1.0f);
    vert[32].position = vec3(-1.0f, 1.0f, 1.0f);
    vert[33].position = vec3(1.0f, 1.0f, 1.0f);
    vert[34].position = vec3(-1.0f, 1.0f, 1.0f);
    vert[35].position = vec3(1.0f,-1.0f, 1.0f);

    RenderData* renderData = new RenderData();
    glGenVertexArrays(1, &(renderData->vertexArray));
    glBindVertexArray(renderData->vertexArray);
    {
        glGenBuffers(1, &(renderData->vertexBuffer));
        glBindBuffer(GL_ARRAY_BUFFER, renderData->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
        {
            glVertexAttribPointer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            //glVertexAttribPointer(VertexArray::Attrib::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(VertexArray::Attrib::POSITION);
        //glEnableVertexAttribArray(VertexArray::Attrib::COLOR);
    }
    glBindVertexArray(0);

    renderData->size = 36;

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

void ResourcesMgr::loadTextures()
{
    uint32 textureId = gli::createTexture2D("../res/textures/test.tga");
    if (textureId == 0)
    {
        std::cout << "There is no such file." << std::endl;
        return;
    }

    textures["test"] = textureId;
}
