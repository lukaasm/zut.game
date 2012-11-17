#include "Text2d.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Camera.h"
#include "GameObject.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"
#include "VertexArrayObject.h"
#include "Shader.h"

// warning C4482: nonstandard extension used: enum 'VertexArray::Attrib' used in qualified name
#pragma warning(disable : 4482)

Text2D::~Text2D()
{
    delete shader;
}

struct FontVert
{
    glm::vec2 pos;
    glm::vec2 uv;
};

void Text2D::Init()
{
    shader = new Shader("../res/shaders/font.vert", "../res/shaders/font.frag");

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

    glDisable(GL_DEPTH_TEST);

    // Bind shader
    shader->Bind();

    rd->ActivateTexture(GL_TEXTURE0, textureId);
    rd->SetUniforms(shader);

    rd->DrawTriangles(vao);

    shader->Unbind();

    glEnable(GL_DEPTH_TEST);
}
