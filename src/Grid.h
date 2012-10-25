/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_GRID
#define H_GRID

#include <glm/glm.hpp>

#include "Common.h"
#include "GameObject.h"

using namespace glm;

class RenderDevice;
class Shader;

class Grid : GameObject
{
    public:
        Grid();
        ~Grid();

        void OnRender(RenderDevice*) override;

    private:
        uint32 _size;

        Shader* _shader;
};

#endif
