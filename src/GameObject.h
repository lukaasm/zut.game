/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_ENTITY
#define H_ENTITY

#include <glm/glm.hpp>
#include <string>

#include "Common.h"

using namespace glm;

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

    vec3 position;
    vec2 uv;
    vec3 normal;
    vec3 color;
};

#define UV_VERTEX_POS sizeof(vec3)
#define NORMAL_VERTEX_POS UV_VERTEX_POS + sizeof(vec2)
#define COLOR_VERTEX_POS NORMAL_VERTEX_POS + sizeof(vec3)

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

        mat4 GetModelMatrix();

        void SetGuid(uint32 guid) { this->guid = guid; }

        void SetPosition(const vec3& pos)
        {
            position = pos;
        }

        void SetScale(const vec3& scale)
        {
            this->scale = scale;
        }

        std::string GetModel() const { return modelName; }
        std::string GetTexture() const { return textureName; }

    protected:
        uint32 guid;

        std::string modelName;
        std::string textureName;

        vec3 scale;
        vec3 position;

        float rotationX;
        //float rotationY;
};

#endif
