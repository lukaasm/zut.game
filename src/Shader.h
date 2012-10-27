/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_SHADER
#define H_SHADER

#include <string>

#include "Common.h"

class Shader
{
    public:
        Shader(std::string, std::string);
        ~Shader();

        void Bind();
        void Unbind();

        char* ValidiateShader(uint32);

        uint32 GetId() const { return id; }

        uint32 GetMVPMatrixLocation() const { return mvpLoc; }

    private:
        void prepareShader(std::string, uint32&, uint32&);

        uint32 id;

        uint32 vertShader;
        uint32 fragShader;

        // matrixes locations for uniforms
        uint32 mvpLoc;

    public:
        uint32 textEnabledLoc;
        uint32 textLoc;
};

#endif
