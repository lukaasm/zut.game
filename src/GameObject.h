/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_ENTITY
#define H_ENTITY

#include <glm/glm.hpp>

namespace VertexArray
{
    enum Attrib
    {
        POSITION = 0,
        COLOR    = 3,
        TEXCOORD = 4
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

#endif
