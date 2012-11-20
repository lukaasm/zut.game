#ifndef H_MODELDATA
#define H_MODELDATA

#include <string>
#include <vector>

#include "Common.h"
#include "VertexArrayObject.h"

class BoundingBoxProto;
struct Vertex;

struct ModelData
{
    std::string modelName;
    std::string textureName;

    uint32 textureId;

    BoundingBoxProto* boundingBox;
    VertexArrayObject vao;

    typedef std::vector<Vertex> VertexVector;
    VertexVector vertexes;
};

#endif
