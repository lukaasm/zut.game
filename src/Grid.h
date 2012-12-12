/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_GRID
#define H_GRID

#include <glm/glm.hpp>

#include "Common.h"
#include "GameObject.h"

class Shader;

class Grid : public GameObject
{
    public:
        Grid();

        void OnRender() override;
};

#endif
