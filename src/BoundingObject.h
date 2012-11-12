#ifndef H_BOUNDINGOBJECT
#define H_BOUNDINGOBJECT

#include <glm/glm.hpp>
#include <vector>

#include "VertexArrayObject.h"

class BoundingBox;
class Frustum;
//class GameObject;
class RenderDevice;

struct Vertex;

class BoundingObject
{
    public:
        //GameObject* GetOwner() const { return owner; }

        virtual void OnRender(RenderDevice*) = 0;

        virtual bool Intersection(const glm::vec3&) const = 0;
        virtual bool Intersection(const BoundingBox&) const = 0;
        virtual bool Intersection(const Frustum&) const = 0;
        //virtual bool IsInFrustum(const Frustum&, GameObject*) = 0;

    private:
        //GameObject* owner;
};

typedef std::vector<Vertex> VertexVector;

struct TestPoints;

class BoundingBox : public BoundingObject
{
    friend struct TestPoints;
    public:
        void SetMinMax(const VertexVector&);

        void OnRender(RenderDevice*);

        bool Intersection(const glm::vec3& point) const override { return false; }
        bool Intersection(const BoundingBox& box) const override;
        bool Intersection(const Frustum& frustum) const override{ return false; }

        static bool Intersection(TestPoints&, TestPoints&);
    private:
        glm::vec3 min;
        glm::vec3 max;

        VertexArrayObject vao;
};

struct TestPoints
{
    TestPoints(BoundingBox& bb, glm::mat4 modelMatrix)
    {
        glm::vec4 m1 = modelMatrix * glm::vec4(bb.min, 1.0f);
        glm::vec4 m2 = modelMatrix * glm::vec4(bb.max, 1.0f);
        min = glm::vec3(m1.x, m1.y, m1.z);
        max = glm::vec3(m2.x, m2.y, m2.z);;
    }

    glm::vec3 min;
    glm::vec3 max;
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
