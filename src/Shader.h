/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_SHADER
#define H_SHADER

#include <string>
#include <unordered_map>
#include <vector>

#include "Common.h"
#include "Light.h"

typedef std::unordered_map<std::string, uint32> UniformsMap;

class Shader
{
    public:
        Shader() {}
        ~Shader();

        void Bind();
        void Unbind();

        void AddAttribute(uint32, std::string);

        void AddUniform(std::string);
        void AddLightSources(uint32);

        void SetLightSources(std::vector<LightSource>&);

        void SetUniform(std::string, glm::mat4);
        void SetUniform(std::string, glm::mat3);
        void SetUniform(std::string, glm::vec4);
        void SetUniform(std::string, glm::vec3);
        void SetUniform(std::string, glm::vec2);
        void SetUniform(std::string, float);
        void SetUniform(std::string, int);

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
