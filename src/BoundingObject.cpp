#include "BoundingObject.h"

#include <algorithm>
#include <GL/glew.h>

#include "Common.h"
#include "GameObject.h"
#include "RenderDevice.h"

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

    renderData = new RenderData();
    glGenVertexArrays(1, &(renderData->vertexArray));
    glBindVertexArray(renderData->vertexArray);
    {
        glGenBuffers(1, &(renderData->vertexBuffer));
        glBindBuffer(GL_ARRAY_BUFFER, renderData->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, bbox.size()*sizeof(glm::vec3), &bbox[0], GL_STATIC_DRAW);
        {
            glVertexAttribPointer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
            glVertexAttribPointer(VertexArray::Attrib::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(VertexArray::Attrib::POSITION);
        glEnableVertexAttribArray(VertexArray::Attrib::COLOR);
    }
    glBindVertexArray(0);

    renderData->size = bbox.size();
}

bool BoundingBox::IsInFrustum(const Frustum& frustum, GameObject* owner)
{
    glm::mat4 modelMatrix = owner->GetModelMatrix();
    glm::vec4 min = glm::vec4(this->min, 1.0f) * modelMatrix;
    glm::vec4 max = glm::vec4(this->max, 1.0f) * modelMatrix;

    return true;
}

void BoundingBox::OnRender(RenderDevice* rd)
{
    rd->DrawLines(renderData->vertexArray, 0, renderData->size);
}

bool BoundingSphere::IsInFrustum(const Frustum& frustum, GameObject* owner)
{

}
