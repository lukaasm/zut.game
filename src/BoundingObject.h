#ifndef H_BOUNDINGOBJECT
#define H_BOUNDINGOBJECT

#include <glm/glm.hpp>
#include <vector>

class Frustum;
class GameObject;
class RenderDevice;

struct Vertex;
struct RenderData;

class BoundingObject
{
    public:
        GameObject* GetOwner() const { return owner; }

        virtual void OnRender(RenderDevice*) = 0;
        virtual bool IsInFrustum(Frustum&, GameObject*) = 0;

    private:
        GameObject* owner;
};

typedef std::vector<Vertex> VertexVector;

class BoundingSphere;

class BoundingBox : public BoundingObject
{
    public:
        void SetMinMax(VertexVector&);

        void OnRender(RenderDevice*);
        bool Collision(BoundingBox& box) { return false; }
        bool Collision(BoundingSphere& sphere) { return false; }

        bool IsInFrustum(Frustum& frustum, GameObject*) override;

    private:
        glm::vec3 min;
        glm::vec3 max;

        RenderData* renderData;
};

class BoundingSphere : public BoundingObject
{
    public:
        bool Collision(BoundingBox& box) { return false; }
        bool Collision(BoundingSphere& sphere) { return false; }

        bool IsInFrustum(Frustum& frustum, GameObject*) override;
};

#endif