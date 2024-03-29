/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Shader.h"

#include <cstdlib>
#include <cstring>  // for memset
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Exception.h"
#include "GameObject.h"
#include "Light.h"

Shader::~Shader()
{
    glDetachShader(id, fragShader);
    glDetachShader(id, vertShader);

    glDeleteShader(fragShader);
    glDeleteShader(vertShader);
    glDeleteProgram(id);
}

void Shader::Bind()
{
    glUseProgram(id);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

std::string Shader::getShaderInfo(uint32 id)
{
    const uint32 BUFFER_SIZE = 512;

    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    std::string out;
    glGetShaderInfoLog(id, BUFFER_SIZE, &length, buffer);
    if (length > 0)
        out = buffer;

    return out;
}

Shader* Shader::LoadFromFile(std::string fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
        throw Exception("[Shader] problem occurred while trying to open file: " + fileName);

    std::cout << "[Shader] loading file: " << fileName << std::endl;
    id = glCreateProgram();

    while (!file.eof())
    {
        char buff[1000];
        file.getline(buff, 1000);

        std::string line(buff);
        if (line.find("//") == 0)
        {
            // it is a comment, ignore
            continue;
        }
        else if (line.find("#vert_start") == 0)
        {
            // vertex shader data start
            std::string data = loadShaderData(file, "#vert");

            vertShader = glCreateShader(GL_VERTEX_SHADER);

            const char* temp = data.c_str();
            glShaderSource(vertShader, 1, &temp, 0);
            glCompileShader(vertShader);

            std::cout << "Shader info (#vert_start): " << getShaderInfo(vertShader) << std::endl;

            glAttachShader(GetId(), vertShader);
        }
        else if (line.find("#frag_start") == 0)
        {
            // fragment shader data start
            std::string data = loadShaderData(file, "#frag");

            fragShader = glCreateShader(GL_FRAGMENT_SHADER);

            const char* temp = data.c_str();
            glShaderSource(fragShader, 1, &temp, 0);
            glCompileShader(fragShader);

            std::cout << "Shader info (#frag_start): " <<getShaderInfo(fragShader) << std::endl;

            glAttachShader(GetId(), fragShader);
        }
    }
    glLinkProgram(GetId());
    return this;
}

std::string Shader::loadShaderData(std::ifstream& file, std::string data)
{
    std::string shaderData;
    while (!file.eof())
    {
        char buff[1000];
        file.getline(buff, 1000);

        std::string line(buff);
        if (line.find(data + "_end") == 0)
            return shaderData;

        shaderData.append(line).append("\n");
    }

    throw Exception("[Shader] there is NO data terminator for shader:" + data);
}

uint32 Shader::GetUniformLocation(std::string key)
{
    auto i = uniformsLocation.find(key);
    if (i != uniformsLocation.end())
        return i->second;

    throw Exception("[Shader][E] there is NO such uniform for this shader " + key);
}

void Shader::AddAttribute(uint32 location, std::string key)
{
    glBindAttribLocation(GetId(), location, key.c_str());
}

void Shader::AddUniform(std::string key)
{
    int loc = glGetUniformLocation(GetId(), key.c_str());
    if (loc == 0xFF)
    {
        std::cout << key << " no such uniform in shader" << std::endl;
        return;
    }

    uniformsLocation[key] = loc;
}

void Shader::SetUniform(std::string key, glm::mat4 matrix)
{
    glUniformMatrix4fv(GetUniformLocation(key), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniform(std::string key, glm::mat3 matrix)
{
    glUniformMatrix3fv(GetUniformLocation(key), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniform(std::string key, glm::vec4 vector)
{
    glUniform4fv(GetUniformLocation(key), 1, glm::value_ptr(vector));
}

void Shader::SetUniform(std::string key, glm::vec3 vector)
{
    glUniform3fv(GetUniformLocation(key), 1, glm::value_ptr(vector));
}

void Shader::SetUniform(std::string key, glm::vec2 vector)
{
    glUniform2fv(GetUniformLocation(key), 1, glm::value_ptr(vector));
}

void Shader::SetUniform(std::string key, float value)
{
    glUniform1f(GetUniformLocation(key), value);
}

void Shader::SetUniform(std::string key, int value)
{
    glUniform1i(GetUniformLocation(key), value);
}

void Shader::AddDirectionalLight()
{
    AddUniform("in_Light.Direction");
    AddUniform("in_Light.Color");
}

void Shader::SetDirectionalLight(glm::vec3 direction, glm::vec3 color)
{
    SetUniform("in_Light.Direction", direction);
    SetUniform("in_Light.Color", color);
}

void Shader::AddPointLight()
{
    AddUniform("in_Light.Position");
    AddUniform("in_Light.Color");
    AddUniform("in_Light.Radius");
    AddUniform("in_Light.Intensity");
}

void Shader::SetPointLight(glm::vec3 position, glm::vec3 color, float radius, float intensity)
{
    SetUniform("in_Light.Position", position);
    SetUniform("in_Light.Color", color);
    SetUniform("in_Light.Radius", radius);
    SetUniform("in_Light.Intensity", intensity);
}

void Shader::AddFragDataLocation(uint32 location, std::string name)
{
    glBindFragDataLocation(GetId(), location, name.c_str());
}
