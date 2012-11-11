#include "BoundingObject.h"

#include <algorithm>
#include <GL/glew.h>

#include "Common.h"
#include "GameObject.h"
#include "RenderDevice.h"

// warning C4482: nonstandard extension used: enum 'VertexArray::Attrib' used in qualified name
#pragma warning(disable : 4482)

void BoundingBox::SetMinMax(const VertexVector& vertexes)
{
    struct FindMinMax
    {
        FindMinMax(glm::vec3& var, glm::vec3& var2) : min(var), max(var2) {}
        void operator()(const Vertex& vert)
        {
            for (uint8 i = 0; i < 3; ++i)
            {
                if (vert.position[i] < min[i])
                    min[i] = vert.position[i];

                if (vert.position[i] > max[i])
                    max[i] = vert.position[i];
            }
        }

        private:
            glm::vec3& min;
            glm::vec3& max;
    };

    min = vertexes.front().position;
    max = vertexes.front().position;

    std::for_each(vertexes.begin(), vertexes.end(), FindMinMax(min, max));
    min -= 0.1;
    max += 0.1f;

    std::vector<glm::vec3> bbox;
    bbox.push_back(glm::vec3(min.x, min.y, min.z));
    bbox.push_back(glm::vec3(max.x, min.y, min.z));
    bbox.push_back(glm::vec3(max.x, min.y, min.z));
    bbox.push_back(glm::vec3(max.x, max.y, min.z));
    bbox.push_back(glm::vec3(max.x, max.y, min.z));
    bbox.push_back(glm::vec3(min.x, max.y, min.z));
    bbox.push_back(glm::vec3(min.x, max.y, min.z));
    bbox.push_back(glm::vec3(min.x, min.y, min.z));
    bbox.push_back(glm::vec3(min.x, max.y, max.z));
    bbox.push_back(glm::vec3(max.x, max.y, max.z));
    bbox.push_back(glm::vec3(max.x, max.y, max.z));
    bbox.push_back(glm::vec3(max.x, min.y, max.z));
    bbox.push_back(glm::vec3(max.x, min.y, max.z));
    bbox.push_back(glm::vec3(min.x, min.y, max.z));
    bbox.push_back(glm::vec3(min.x, min.y, max.z));
    bbox.push_back(glm::vec3(min.x, max.y, max.z));

    bbox.push_back(glm::vec3(min.x, max.y, min.z));
    bbox.push_back(glm::vec3(min.x, max.y, max.z));

    bbox.push_back(glm::vec3(max.x, max.y, min.z));
    bbox.push_back(glm::vec3(max.x, max.y, max.z));

    bbox.push_back(glm::vec3(min.x, min.y, min.z));
    bbox.push_back(glm::vec3(min.x, min.y, max.z));

    bbox.push_back(glm::vec3(max.x, min.y, min.z));
    bbox.push_back(glm::vec3(max.x, min.y, max.z));

    vao.CreateVertexArray();
    vao.CreateVertexBuffer();

    vao.Bind(ID_VAO);
    vao.Bind(ID_VBO);
    vao.FillBuffer(GL_STATIC_DRAW, &bbox[0], bbox.size()*sizeof(glm::vec3));
    vao.EnableAttrib(VertexArray::Attrib::POSITION);
    vao.AddAttribToBuffer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    vao.EnableAttrib(VertexArray::Attrib::COLOR);
    vao.AddAttribToBuffer(VertexArray::Attrib::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    vao.Unbind(ID_VBO);
    vao.Unbind(ID_VAO);

    vao.ElementsCount() = bbox.size();
}

void BoundingBox::OnRender(RenderDevice* rd)
{
    rd->DrawLines(vao);
}

bool BoundingBox::Intersection(const BoundingBox& box) const 
{
    if (min.x > box.max.x)
        return false;

    if (box.min.x > max.x)
        return false;

    if (min.y > box.max.y)
        return false;

    if (box.min.y > max.y)
        return false;

    if (min.z > box.max.z)
        return false;

    if (box.min.z > max.z)
        return false;

    return true;
}

bool BoundingBox::Intersection( TestPoints& a, TestPoints& b)
{

    if (a.min.x > b.max.x)
        return false;

    if (b.min.x > a.max.x)
        return false;

    if (a.min.y > b.max.y)
        return false;

    if (b.min.y > a.max.y)
        return false;

    if (a.min.z > b.max.z)
        return false;

    if (b.min.z > a.max.z)
        return false;

    return true;
}
