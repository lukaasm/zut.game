#include "Text2d.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Camera.h"
#include "GameObject.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"
#include "Shader.h"

Text2D::~Text2D()
{
    // Delete buffers
    glDeleteVertexArrays(1, &(renderData->vertexArray));
    glDeleteBuffers(1, &(renderData->vertexBuffer));

    delete shader;
}

void Text2D::Init()
{
    shader = new Shader("../res/shaders/font.vert", "../res/shaders/shader.frag");

    // Initialize texture
    textureId = sResourcesMgr->GetTextureId("font");

    renderData = new RenderData;

    // Initialize VBO
    glGenVertexArrays(1, &(renderData->vertexArray)); 
    glGenBuffers(1, &(renderData->vertexBuffer));
}

struct FontVert
{
    vec2 pos;
    vec2 uv;
};

#include <iostream>

void Text2D::Print(RenderDevice* rd, std::string text, int x, int y, int fontSize)
{
    // Fill buffers
    std::vector<FontVert> vertices;

    uint32 size = fontSize;
    for (uint32 i = 0 ; i < text.size() ; ++i )
    {
        char character = text.c_str()[i];
        float uv_x = (character % 16)/16.0f;
        float uv_y = (character / 16)/16.0f;

        FontVert vert[6] =
        {
            { vec2(x+i*size     , y+size), vec2(uv_x           , 1.0f - uv_y) },
            { vec2(x+i*size     , y     ), vec2(uv_x           , 1.0f - (uv_y + 1.0f/16.0f)) },
            { vec2(x+i*size+size, y+size), vec2(uv_x+1.0f/16.0f, 1.0f - uv_y) },
            { vec2(x+i*size+size, y     ), vec2(uv_x+1.0f/16.0f, 1.0f - (uv_y + 1.0f/16.0f)) },
            { vec2(x+i*size+size, y+size), vec2(uv_x+1.0f/16.0f, 1.0f - uv_y) },
            { vec2(x+i*size     , y     ), vec2(uv_x           , 1.0f - (uv_y + 1.0f/16.0f)) },
        };

        for (uint8 j = 0; j < 6; ++j)
            vertices.push_back(vert[j]);
    }

    renderData->size = vertices.size();

    glBindVertexArray(renderData->vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, renderData->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(FontVert)*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
    {
        glVertexAttribPointer(VertexArray::Attrib::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(FontVert), 0);
        glVertexAttribPointer(VertexArray::Attrib::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(FontVert), BUFFER_OFFSET(sizeof(vec2)));
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(VertexArray::Attrib::POSITION);
    glEnableVertexAttribArray(VertexArray::Attrib::TEXCOORD);

    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);

    // Bind shader
    shader->Bind();

    rd->ActivateTexture(GL_TEXTURE0, textureId);
    rd->SetUniforms(shader);

    rd->DrawTriangles(renderData->vertexArray, 0, renderData->size);

    shader->Unbind();

    glEnable(GL_DEPTH_TEST);
}
