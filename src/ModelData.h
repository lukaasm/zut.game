#ifndef H_MODELDATA
#define H_MODELDATA

#include <string>
#include <vector>

#include "Common.h"
#include "VertexArrayObject.h"

class BoundingBoxProto;
struct Vertex;

typedef std::vector<Vertex> VertexVector;

class ModelData
{
    public:
        ~ModelData();

        static ModelData* LoadModel(std::string fileName);

        void Render();
   // private:
        uint32 objectsCount;

        BoundingBoxProto* boundingBox;

        uint32* textureId;
        VertexArrayObject* vao;
        VertexVector* vertexes;
};

#endif
