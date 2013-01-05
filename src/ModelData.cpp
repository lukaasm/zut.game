#include "ModelData.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>

#include "GameObject.h"
#include "RenderDevice.h"

glm::vec3 ai2glm(const aiVector3D& vec)
{
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec3 ai2glm(const aiColor4D& vec)
{
    return glm::vec3(vec.r, vec.g, vec.b);
}

ModelData* ModelData::LoadModel(std::string fileName)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (scene == nullptr)
        return nullptr;

    ModelData* md = new ModelData;

    md->objectsCount = scene->mNumMeshes;

    md->vao = new VertexArrayObject[md->objectsCount];
    md->vertexes = new VertexVector[md->objectsCount];
    md->boundingBox = new BoundingBoxProto[md->objectsCount];

    glm::vec3 vecZero = glm::vec3(0.0f, 0.0f, 0.0f);
    for (uint8 i = 0; i < md->objectsCount; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[i];
        for (uint32 j = 0; j < mesh->mNumVertices; ++j)
        {
            Vertex vertex;
            vertex.position = ai2glm(mesh->mVertices[j]);
            vertex.normal = mesh->HasNormals() ? ai2glm(mesh->mNormals[j]) : vecZero;
            vertex.color = mesh->HasVertexColors(0) ? ai2glm(mesh->mColors[0][j]) : vecZero;
            vertex.uv = mesh->HasTextureCoords(0) ? glm::vec2(ai2glm(mesh->mTextureCoords[0][j])) : glm::vec2(0.0f, 0.0f);

            md->vertexes[i].push_back(vertex);
        }

        md->boundingBox[i].SetMinMax(md->vertexes[i]);

        md->vao[i].CreateVertexArray();
        md->vao[i].CreateVertexBuffer();

        md->vao[i].Bind(ID_VAO);
        md->vao[i].Bind(ID_VBO);
        md->vao[i].FillBuffer(GL_STATIC_DRAW, &(md->vertexes[i][0]), md->vertexes[i].size()*sizeof(Vertex));

        md->vao[i].EnableAttrib(VertexArray::Attrib::POSITION);
        md->vao[i].AddAttribToBuffer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

        if (mesh->HasNormals())
        {
            md->vao[i].EnableAttrib(VertexArray::Attrib::NORMAL);
            md->vao[i].AddAttribToBuffer(VertexArray::Attrib::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(NORMAL_VERTEX_POS));
        }

        if (mesh->HasVertexColors(0))
        {
            md->vao[i].EnableAttrib(VertexArray::Attrib::COLOR);
            md->vao[i].AddAttribToBuffer(VertexArray::Attrib::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(COLOR_VERTEX_POS));
        }

        if (mesh->HasTextureCoords(0))
        {
            md->vao[i].EnableAttrib(VertexArray::Attrib::TEXCOORD);
            md->vao[i].AddAttribToBuffer(VertexArray::Attrib::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(UV_VERTEX_POS));
        }

        md->vao[i].Unbind(ID_VBO);
        md->vao[i].Unbind(ID_VAO);

        md->vao[i].ElementsCount() = md->vertexes[i].size();
    }
    return md;
}

void ModelData::Render()
{
    //OGLHelper::ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId("placeholder.tga"));

    for (uint8 i = 0; i < objectsCount; ++i)
    {
        // texture
        OGLHelper::DrawTriangles(vao[i]);
    }
}

ModelData::~ModelData()
{
    delete [] vao;
    delete [] vertexes;
}
