#ifndef H_BOUNDINGOBJECT
#define H_BOUNDINGOBJECT

#include <glm/glm.hpp>
#include <vector>

#include "VertexArrayObject.h"

class AABoundingBox;
class Frustum;

struct Vertex;

enum BoundingType
{
    BOUNDING_AABOX     = 0,
    BOUNDING_BOX_PROTO = 1,
};

typedef std::vector<Vertex> VertexVector;

class BoundingBoxProto
{
    friend class AABoundingBox;
    public:
        void SetMinMax(const VertexVector&);

        BoundingType GetType() const { return BOUNDING_BOX_PROTO; }

        void OnRender();

        //virtual bool Intersection(const glm::vec3& point) const = 0;
        //virtual bool Intersection(const AABoundingBox& box) const = 0;
        //virtual bool Intersection(const Frustum& frustum) const = 0;

    protected:
        glm::vec3 min;
        glm::vec3 max;

        VertexArrayObject vao;
};

/*
class BoundingSphere : public BoundingObject
{
    public:
        bool Collision(const BoundingBox& box) { return false; }
        bool Collision(const BoundingSphere& sphere) { return false; }

        bool IsInFrustum(const Frustum& frustum, GameObject*) override;
};
*/

#endif
