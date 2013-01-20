#include "Text2d.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Camera.h"
#include "Config.h"
#include "GameObject.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"
#include "VertexArrayObject.h"
#include "Shader.h"

Text2D::~Text2D()
{
}

struct FontVert
{
    glm::vec2 pos;
    glm::vec2 uv;
};

void Text2D::Init()
{
    // Initialize texture
    textureId = sResourcesMgr->GetTextureId("font.tga");

    // Initialize VBO
    vao.CreateVertexArray();
    vao.CreateVertexBuffer();

    vao.Bind(ID_VAO);
    vao.Bind(ID_VBO);
    vao.EnableAttrib(VertexArray::Attrib::POSITION);
    vao.AddAttribToBuffer(VertexArray::Attrib::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(FontVert), 0);

    vao.EnableAttrib(VertexArray::Attrib::TEXCOORD);
    vao.AddAttribToBuffer(VertexArray::Attrib::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(FontVert), BUFFER_OFFSET(sizeof(glm::vec2)));

    vao.Unbind(ID_VBO);
    vao.Unbind(ID_VAO);
}

void Text2D::RenderText(std::string text, int x, int y, int fontSize)
{
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
            { glm::vec2(x+i*size     , y+size), glm::vec2(uv_x           , 1.0f - uv_y) },
            { glm::vec2(x+i*size     , y     ), glm::vec2(uv_x           , 1.0f - (uv_y + 1.0f/16.0f)) },
            { glm::vec2(x+i*size+size, y+size), glm::vec2(uv_x+1.0f/16.0f, 1.0f - uv_y) },
            { glm::vec2(x+i*size+size, y     ), glm::vec2(uv_x+1.0f/16.0f, 1.0f - (uv_y + 1.0f/16.0f)) },
            { glm::vec2(x+i*size+size, y+size), glm::vec2(uv_x+1.0f/16.0f, 1.0f - uv_y) },
            { glm::vec2(x+i*size     , y     ), glm::vec2(uv_x           , 1.0f - (uv_y + 1.0f/16.0f)) },
        };

        for (uint8 j = 0; j < 6; ++j)
            vertices.push_back(vert[j]);
    }

    vao.ElementsCount() = vertices.size();

    vao.Bind(ID_VAO);
    vao.Bind(ID_VBO);

    vao.FillBuffer(GL_DYNAMIC_DRAW, &vertices[0], sizeof(FontVert)*vertices.size());

    vao.Unbind(ID_VBO);
    vao.Unbind(ID_VAO);

    Shader* shader = sResourcesMgr->GetShader("text2d.glsl");

    // Bind shader

    int w = sConfig->GetDefault("width", WINDOW_WIDTH);
    int h = sConfig->GetDefault("height", WINDOW_HEIGHT);

    shader->Bind();

    shader->SetUniform("in_ScreenHWidth", w/2);
    shader->SetUniform("in_ScreenHHeight", h/2);

    OGLHelper::ActivateTexture(GL_TEXTURE0, textureId);

    OGLHelper::DrawTriangles(vao);

    shader->Unbind();
    glDisable(GL_BLEND);
}

void Text2D::RenderSprite(int x, int y, int size, int textureId)
{
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Fill buffers
    std::vector<FontVert> vertices;
    FontVert vert[6] =
    {
        { glm::vec2(x     , y+size), glm::vec2(0,1) },
        { glm::vec2(x     , y     ), glm::vec2(0,0) },
        { glm::vec2(x+size, y+size), glm::vec2(1,1) },
        { glm::vec2(x+size, y     ), glm::vec2(1,0) },
        { glm::vec2(x+size, y+size), glm::vec2(1,1) },
        { glm::vec2(x     , y     ), glm::vec2(0,0) },
    };

    for (uint8 j = 0; j < 6; ++j)
        vertices.push_back(vert[j]);

    vao.ElementsCount() = vertices.size();

    vao.Bind(ID_VAO);
    vao.Bind(ID_VBO);

    vao.FillBuffer(GL_DYNAMIC_DRAW, &vertices[0], sizeof(FontVert)*vertices.size());

    vao.Unbind(ID_VBO);
    vao.Unbind(ID_VAO);

    Shader* shader = sResourcesMgr->GetShader("text2d.glsl");

    // Bind shader
    shader->Bind();

    OGLHelper::ActivateTexture(GL_TEXTURE0, textureId);

    OGLHelper::DrawTriangles(vao);

    shader->Unbind();
    glDisable(GL_BLEND);
}

void Text2D::RenderSprite(VertexArrayObject& vao, int textureId)
{
    Shader* shader = sResourcesMgr->GetShader("text2d.glsl");

    // Bind shader
    shader->Bind();

    OGLHelper::ActivateTexture(GL_TEXTURE0, textureId);

    OGLHelper::DrawTriangles(vao);

    shader->Unbind();
}
