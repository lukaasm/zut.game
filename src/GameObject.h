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
        explicit GameObject(std::string model, std::string texture) : coll(0.0f), modelName(model), textureName(texture), boundingObject(nullptr) {}
        virtual ~GameObject() {}

        virtual void OnRender(RenderDevice*);
        virtual void OnUpdate(const uint32) {}

        glm::mat4 GetModelMatrix() const;

        void SetGuid(uint32 guid);
        void SetBoundingObject(BoundingObject* object);
        BoundingObject* GetBoundingObject() const { return boundingObject; }

        void SetPosition(const Position& pos);
        void SetScale(const glm::vec3& scale);

        std::string GetModel() const { return modelName; }
        std::string GetTexture() const { return textureName; }

        float IsTextured() const { return GetTexture() != "" ? 1.0f : 0.0f; }
        float coll;

    protected:
        uint32 guid;

        std::string modelName;
        std::string textureName;

        glm::vec3 scale;
        glm::vec3 position;

        float rotationX;
        //float rotationY;

        BoundingObject* boundingObject;
};

#endif
