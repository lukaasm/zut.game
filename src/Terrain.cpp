#include "Terrain.h"

//#include <gl/glfw.h>
#include <algorithm>
#include <vector>

#include "Common.h"
#include "Exception.h"
#include "GameObject.h"
#include "RenderDevice.h"

#define SCALE 1.0f

Terrain::Terrain()
{
    GLFWimage image;
    if (glfwReadImage("../res/textures/terrain.tga", &image, 0) == GL_FALSE)
        throw Exception("[Terrain][E] cannot read terrain data from texture.");

    terrainHeights = new float[image.Width * image.Height];

    terrainSize.x = image.Width;
    terrainSize.y = image.Height;

    for (int y = 0; y < image.Height; y++)
    {
        for (int x = 0; x < image.Width; x++)
        {
            int bpp = image.BytesPerPixel;
            int pitch = image.Width * bpp;

            Vertex vertex;

            uint8 height = image.Data[y * pitch + x * bpp];

            float fHeight = float(height) / 6.0f;
            terrainHeights[y * image.Width + x] = fHeight;

            vertVector.push_back(glm::vec3(float(x) * SCALE, fHeight, float(y) * SCALE));
            texVector.push_back(glm::vec2(float(x) / float(image.Width), float(y) / float(image.Height)));
        }
    }

    calculateFaces(&image);
    calculateNormals();

    createVAO();

    glfwFreeImage(&image);
}

Terrain::~Terrain()
{
    delete [] terrainHeights;
}

void Terrain::OnRender(RenderDevice* rd)
{
    rd->DrawTriangles(vao);
}

void Terrain::calculateFaces(GLFWimage* image)
{
    for (int y = 0; y < image->Height-1; y++)
    {
        for (int x = 0; x < image->Width-1; x++)
        {
            PolygonFace tri1, tri2;
            int v1 = y * image->Width + x;
            int v2 = v1 + 1;
            int v3 = v1 + image->Width;
            int v4 = v3 + 1;

            tri1.addIndexes(v1, v1, v1);
            tri1.addIndexes(v3, v3, v3);
            tri1.addIndexes(v2, v2, v2);
            tri2.addIndexes(v2, v2, v2);
            tri2.addIndexes(v3, v3, v3);
            tri2.addIndexes(v4, v4, v4);

            faceVector.push_back(tri1);
            faceVector.push_back(tri2);
        }
    }
}

void Terrain::calculateNormals()
{
    std::vector<glm::vec3>* normal_buffer = new std::vector<glm::vec3>[vertVector.size()];
    for (unsigned int i=0; i<faceVector.size(); i++)
    {
        PolygonFace& face = faceVector[i];
        assert(face.v.size() == 3);
        glm::vec3& gv1 = vertVector[face.v[0]];
        glm::vec3& gv2 = vertVector[face.v[1]];
        glm::vec3& gv3 = vertVector[face.v[2]];

        glm::vec3 normal = glm::normalize(glm::cross(gv1 - gv2, gv1 - gv3));
        normal_buffer[face.v[0]].push_back(normal);
        normal_buffer[face.v[1]].push_back(normal);
        normal_buffer[face.v[2]].push_back(normal);
    }

    normVector.clear();

    for (unsigned int i = 0; i < vertVector.size(); i++)
    {
        glm::vec3 normal = normal_buffer[i][0];
        for (unsigned int j = 1; j< normal_buffer[i].size(); j++)
            normal += normal_buffer[i][j];

        normal /= normal_buffer[i].size();
        normVector.push_back(normal);
    }
}

void Terrain::createVAO()
{
    std::vector<Vertex> vertexes;
    for (uint32 i = 0; i < faceVector.size(); i++)
    {
        const PolygonFace& face = faceVector[i];
        for (uint8 j = 0; j < 3; j++)
        {
            Vertex vertex;
            vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
            vertex.position = vertVector[face.v[j]];
            vertex.normal = normVector[face.vn[j]];
            vertex.uv = texVector[face.vt[j]];

            vertexes.push_back(vertex);
        }
    }

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
}

PolygonFace::PolygonFace(const PolygonFace& b)
{
    std::for_each(b.v.begin(), b.v.end(), [this](int i) { this->v.push_back(i); });
    std::for_each(b.vn.begin(), b.vn.end(), [this](int i) { this->vn.push_back(i); });
    std::for_each(b.vt.begin(), b.vt.end(), [this](int i) { this->vt.push_back(i); });
}
