/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#include "Grid.h"

#include <stdio.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.h"
#include "RenderDevice.h"
#include "Shader.h"

using namespace glm;

Grid::Grid(RenderDevice* renderDevice)
{
    _renderDevice = renderDevice;

    _size = 40;

    Vertex vertices[160]; // _size*4

    for (uint32 x = 0; x < _size; x++)
    {
        vertices[x*2].position = vec3(float(x), 0.0f, 0.0f);
        vertices[x*2 +1].position = vec3(float(x), 0.0f, -float(_size));

        vertices[x*2 +_size*2].position = vec3(0.0f, 0.0f, -float(x));
        vertices[x*2 +_size*2+1].position = vec3(float(_size), 0.0f, -float(x));

        vertices[x*2].color = vec3(0.0f, 1.0f, 0.0f);
        vertices[x*2 +1].color = vec3(0.0f, 1.0f, 0.0f);

        vertices[x*2 +_size*2].color = vec3(0.0f, 1.0f, 0.0f);
        vertices[x*2 +_size*2+1].color = vec3(0.0f, 1.0f, 0.0f);
    };

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    {
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        {
            glVertexAttribPointer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            glVertexAttribPointer(VertexArray::Attrib::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(VertexArray::Attrib::POSITION);
        glEnableVertexAttribArray(VertexArray::Attrib::COLOR);
    }
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
    glDrawArrays(GL_LINES, 0, 4*_size);

    _shader->Unbind();
}
