#include "ResourcesMgr.h"

#include <GL/glew.h>

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

    rendersData[name] = renderData;
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