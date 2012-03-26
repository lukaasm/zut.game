/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Grid.h"

#include <stdio.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderDevice.h"
#include "Shader.h"

using namespace glm;

Grid::Grid(RenderDevice* renderDevice)
{
    _renderDevice = renderDevice;

    _size = 40;

    vec3 vertices[160]; // _size*4

    /*vertices[0] = vec3(0.0f, 0.0f, 0.0f);
    vertices[1] = vec3(1.0f, 0.0f, 0.0f);
    vertices[2] = vec3(1.0f, 1.0f, 0.0f);
    vertices[3] = vec3(0.0f, 1.0f, 0.0f);
    vertices[4] = vec3(0.0f, 0.0f, 0.0f);

    vertices[5] = vec3(0.5f, 0.5f, 1.0f);
    vertices[6] = vec3(1.5f, 0.5f, 1.0f);
    vertices[7] = vec3(1.5f, 1.5f, 1.0f);
    vertices[8] = vec3(0.5f, 1.5f, 1.0f);
    vertices[9] = vec3(0.5f, 0.5f, 1.0f);

    vertices[10] = vec3(0.0f, 0.0f, 0.0f);
    vertices[11] = vec3(0.0f, 0.0f, -20.0f);
    */
    for (uint32 x = 0; x < _size; x++)
    {
        vertices[x*2] = vec3(float(x), 0.0f, 0.0f);
        vertices[x*2 +1] = vec3(float(x), 0.0f, -float(_size));

        vertices[x*2 +_size*2] = vec3(0.0f, 0.0f, -float(x));
        vertices[x*2 +_size*2+1] = vec3(float(_size), 0.0f, -float(x));
    };

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    _shader = new Shader("../res/shaders/shader.vert", "../res/shaders/shader.frag");
}

Grid::~Grid()
{
    delete_ptr(_shader)
}

void Grid::OnRender()
{
    _shader->Bind();

    // scale only local matrix
    _modelMatrix = scale(mat4(1.0f), vec3(0.5f));

    _renderDevice->SetUniforms(_shader, _modelMatrix);

    glBindVertexArray(_vao);
    glDrawArrays(GL_LINES, 0, _size*4);

    _shader->Unbind();
}
