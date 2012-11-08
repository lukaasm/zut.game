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
        virtual bool IsInFrustum(const Frustum&, GameObject*) = 0;

    private:
        GameObject* owner;
};

typedef std::vector<Vertex> VertexVector;

class BoundingSphere;

class BoundingBox : public BoundingObject
{
    public:
        void SetMinMax(const VertexVector&);

        void OnRender(RenderDevice*);
        bool Collision(const BoundingBox& box) { return false; }
        bool Collision(const BoundingSphere& sphere) { return false; }

        bool IsInFrustum(const Frustum& frustum, GameObject*) override;

    private:
        glm::vec3 min;
        glm::vec3 max;

        RenderData* renderData;
};

class BoundingSphere : public BoundingObject
{
    public:
        bool Collision(const BoundingBox& box) { return false; }
        bool Collision(const BoundingSphere& sphere) { return false; }

        bool IsInFrustum(const Frustum& frustum, GameObject*) override;
};

#endif
