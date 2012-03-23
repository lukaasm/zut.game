/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_GRID
#define H_GRID

#include "Common.h"

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

        RenderDevice* _renderDevice;
        Shader* _shader;
};

#endif
