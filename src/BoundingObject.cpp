#include "BoundingObject.h"

#include <algorithm>
#include <GL/glew.h>

#include "Common.h"
#include "GameObject.h"
#include "RenderDevice.h"

void BoundingBoxProto::SetMinMax(const VertexVector& vertexes)
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
    //min -= 0.1;
    //max += 0.1f;

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

    vao.Unbind(ID_VBO);
    vao.Unbind(ID_VAO);

    vao.ElementsCount() = bbox.size();
}

void BoundingBoxProto::OnRender()
{
    OGLHelper::DrawLines(vao);
}
