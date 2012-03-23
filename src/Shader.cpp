/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Shader.h"

#include <fstream>
#include <GL/glew.h>
using namespace std;

Shader::Shader(string vertName, string fragName)
{
    _vertShader = 0;
    _fragShader = 0;

    _id = glCreateProgram();

    prepareShader(vertName, _vertShader, _id);
    prepareShader(fragName, _fragShader, _id);


    glBindAttribLocation(GetId(), 0, "in_Position");
    glBindAttribLocation(GetId(), 1, "in_Color");

    glLinkProgram(_id);

    _pLocation = glGetUniformLocation(GetId(), "projMatrix");
    _vLocation = glGetUniformLocation(GetId(), "viewMatrix");
    _mLocation = glGetUniformLocation(GetId(), "modelMatrix");
}

Shader::~Shader()
{
    glDetachShader(_id, _fragShader);
    glDetachShader(_id, _vertShader);

    glDeleteShader(_fragShader);
    glDeleteShader(_vertShader);
    glDeleteProgram(_id);
}

void Shader::prepareShader(string shadName, uint32& subid, uint32& id)
{
    string data = "";
    string line = "";

    ifstream file(shadName, ios::binary);
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

    glAttachShader(id, subid);
}

void Shader::Bind()
{
    glUseProgram(_id);
}

void Shader::Unbind()
{
    glUseProgram(0);
}
