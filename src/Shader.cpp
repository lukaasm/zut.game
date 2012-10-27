/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Shader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <GL/glew.h>

using namespace std;

Shader::Shader(string vertName, string fragName)
{
    vertShader = 0;
    fragShader = 0;

    id = glCreateProgram();

    prepareShader(vertName, vertShader, id);
    prepareShader(fragName, fragShader, id);

    glBindAttribLocation(GetId(), 0, "in_Position");
    glBindAttribLocation(GetId(), 1, "in_TexCoord");
    glBindAttribLocation(GetId(), 3, "in_Color");

    glLinkProgram(id);

    mvpLoc = glGetUniformLocation(GetId(), "mvpMatrix");

    textEnabledLoc = glGetUniformLocation(GetId(), "textureFlag");
    textLoc = glGetUniformLocation(GetId(), "baseTexture");
}

Shader::~Shader()
{
    glDetachShader(id, fragShader);
    glDetachShader(id, vertShader);

    glDeleteShader(fragShader);
    glDeleteShader(vertShader);
    glDeleteProgram(id);
}

void Shader::prepareShader(string shadName, uint32& subid, uint32& id)
{
    string data = "";
    string line = "";

    ifstream file(shadName.c_str(), ios::binary);
    if (file.is_open())
    {
        while (!file.eof())
        {
            getline(file, line);
            data.append(line + "\n");
        }
    }
    else
        exit(EXIT_FAILURE);

    const char* shadText = data.c_str();

    if (shadName.find(".frag") != string::npos)
        subid = glCreateShader(GL_FRAGMENT_SHADER);
    else
        subid = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(subid, 1, &shadText, 0);
    glCompileShader(subid);

    if (char* error = ValidiateShader(id))
    {
        cout << "SHADER ERROR: " << error << endl;
        delete_array(error);
    }

    glAttachShader(id, subid);
}

void Shader::Bind()
{
    glUseProgram(id);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

char* Shader::ValidiateShader(uint32 id)
{
    const unsigned int BUFFER_SIZE = 512;
    char * buffer = new char[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;

    glGetShaderInfoLog(id, BUFFER_SIZE, &length, buffer);
    if (length > 0)
        return buffer;

    delete_array(buffer)
    return nullptr;
}
