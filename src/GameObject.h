/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_ENTITY
#define H_ENTITY

#include <glm/glm.hpp>

#include "Common.h"

using namespace glm;

namespace VertexArray
{
    enum Attrib
    {
        POSITION = 0,
        COLOR    = 3,
    };
};

struct Vertex
{
    Vertex(vec3 pos = vec3(0.0f, 0.0f, 0.0f), vec3 col = vec3(0.0f, 0.0f, 0.0f))
    {
        color = col;
        position = pos;
    };

    vec3 position;
    vec3 color;
};

struct RenderData
{
    uint32 vertexArray;
    uint32 vertexBuffer;
};

class RenderDevice;

class GameObject
{
    public:
        explicit GameObject() {}
        
    //public:
        virtual ~GameObject() {}

        virtual void OnRender(RenderDevice*);

        void SetGuid(uint32 guid) { this->guid = guid; }

    protected:
        uint32 guid;

        mat4 modelMatrix;
};

#endif