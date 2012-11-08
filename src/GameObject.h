/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_ENTITY
#define H_ENTITY

#include <glm/glm.hpp>
#include <string>

#include "Common.h"

namespace VertexArray
{
    enum Attrib
    {
        POSITION = 0,
        TEXCOORD = 1,
        COLOR    = 3,
    };
};

struct Vertex
{
    Vertex() {}
    Vertex(Vertex const& org)
    {
        position = org.position;
        uv = org.uv;
        normal = org.normal;
        color = org.color;
    }

    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::vec3 color;
};

#define UV_VERTEX_POS sizeof(glm::vec3)
#define NORMAL_VERTEX_POS UV_VERTEX_POS + sizeof(glm::vec2)
#define COLOR_VERTEX_POS NORMAL_VERTEX_POS + sizeof(glm::vec3)

class BoundingObject;
class RenderDevice;

struct RenderData
{
    uint32 size;

    uint32 vertexArray;
    uint32 vertexBuffer;

    BoundingObject* bounding;
};

class GameObject
{
    public:
        explicit GameObject(std::string model, std::string texture) : modelName(model), textureName(texture) {}

    //public:
        virtual ~GameObject() {}

        virtual void OnRender(RenderDevice*);

        glm::mat4 GetModelMatrix();

        void SetGuid(uint32 guid) { this->guid = guid; }

        void SetPosition(const glm::vec3& pos)
        {
            position = pos;
        }

        void SetScale(const glm::vec3& scale)
        {
            this->scale = scale;
        }

        std::string GetModel() const { return modelName; }
        std::string GetTexture() const { return textureName; }

    protected:
        uint32 guid;

        std::string modelName;
        std::string textureName;

        glm::vec3 scale;
        glm::vec3 position;

        float rotationX;
        //float rotationY;
};

#endif
