#ifndef H_TERRAIN
#define H_TERRAIN

#include <gl/glew.h>
#include <gl/glfw.h>
#include <glm/glm.hpp>
#include <vector>

#include "VertexArrayObject.h"

class RenderDevice;

struct PolygonFace
{
    PolygonFace() {}
    PolygonFace(const PolygonFace& b);

    void addIndexes(int vi, int ti, int ni)
    {
        v.push_back(vi);
        vt.push_back(ti);
        vn.push_back(ni);
    }

    std::vector<int> v;
    std::vector<int> vt;
    std::vector<int> vn;
};

class Terrain
{
    public:
        Terrain();
        ~Terrain();

        void OnRender(RenderDevice*);

        void calculateFaces(GLFWimage*);
        void calculateNormals();

        void createVAO();

        VertexArrayObject vao;

        std::vector<glm::vec3> vertVector;
        std::vector<glm::vec2> texVector;
        std::vector<glm::vec3> normVector;
        std::vector<PolygonFace> faceVector;

        float* terrainHeights;
        glm::vec2 terrainSize;
};

#endif
