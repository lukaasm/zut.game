/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Grid.h"

#include <stdio.h>
#include <GL/glew.h>

#include "RenderDevice.h"
#include "Shader.h"

Grid::Grid(RenderDevice* renderDevice)
{
    _renderDevice = renderDevice;

    _size = 40;
    glm::vec3 vertices[160]; // _size*4

    for (uint32 x = 0; x < _size; x++)
    {
        vertices[x*2] = glm::vec3(float(x), 0.0f, 0.0f);
        vertices[x*2 +1] = glm::vec3(float(x), _size, 0.0f);

        vertices[x*2 +_size*2] = glm::vec3(0.0f, float(x), 0.0f);
        vertices[x*2 +_size*2+1] = glm::vec3(_size, float(x), 0.0f);
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

    _renderDevice->SetUniforms(_shader);

    glBindVertexArray(_vao);
    glDrawArrays(GL_LINES, 0, _size*4);

    _shader->Unbind();
}
