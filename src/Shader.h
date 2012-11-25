/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_SHADER
#define H_SHADER

#include <string>
#include <unordered_map>

#include "Common.h"

typedef std::unordered_map<std::string, uint32> UniformsMap;

class Shader
{
    public:
        Shader() {}
        ~Shader();

        void Bind();
        void Unbind();

        std::string getShaderInfo(uint32);

        Shader* LoadFromFile(std::string);

        uint32 GetId() const { return id; }

        uint32 GetUniformLocation(std::string key);

    private:
        std::string loadShaderData(std::ifstream&, std::string);

        uint32 id;

        uint32 vertShader;
        uint32 fragShader;

        UniformsMap uniformsLocation;
};

#endif
