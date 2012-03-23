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

        uint32 GetId() const { return _id; }

        uint32 GetProjMatrixLocation() const { return _pLocation; }
        uint32 GetViewMatrixLocation() const { return _vLocation; }
        uint32 GetModelMatrixLocation() const { return _mLocation; }

    private:
        void prepareShader(std::string, uint32&, uint32&);

        uint32 _id;

        uint32 _vertShader;
        uint32 _fragShader;

        // matrixes locations for uniforms
        uint32 _pLocation;
        uint32 _vLocation;
        uint32 _mLocation;
};

#endif
