/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_GRID
#define H_GRID

#include <glm/glm.hpp>

#include "Common.h"

using namespace glm;

class RenderDevice;
class Shader;

class Grid
{
    public:
        Grid(RenderDevice*);
        ~Grid();

        void OnRender();

    private:
        uint32 _vbo;
        uint32 _vao;

        uint32 _size;

        mat4 _modelMatrix;

        RenderDevice* _renderDevice;
        Shader* _shader;
};

#endif
