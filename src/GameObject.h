/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_ENTITY
#define H_ENTITY

#include <glm/glm.hpp>
#include <string>

#include "Common.h"
#include "BoundingObject.h"

namespace VertexArray
{
    enum Attrib
    {
        POSITION = 0,
        TEXCOORD = 1,
        NORMAL   = 2,
        COLOR    = 3,
    };
};

struct Vertex
{
    Vertex()
    {
        color = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    Vertex(Vertex const& org)
    {
        position = org.position;
        uv = org.uv;
        normal = org.normal;
        color = org.color;
    }

    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::vec3 color;
};

#define UV_VERTEX_POS sizeof(glm::vec3)
#define NORMAL_VERTEX_POS UV_VERTEX_POS + sizeof(glm::vec2)
#define COLOR_VERTEX_POS NORMAL_VERTEX_POS + sizeof(glm::vec3)

class GameObject;

class AABoundingBox : public BoundingBoxProto
{
    public:
        AABoundingBox(const BoundingBoxProto& proto, GameObject* o);

        BoundingType GetType() const { return BOUNDING_AABOX; }

        void RecalculateModelMatrix(const Position& pos, const glm::vec3& scale);

        bool Intersection(const AABoundingBox& b);

        glm::mat4& GetModelMatrix() { return modelMatrix; }
        GameObject* GetOwner() { return owner; }

        const glm::vec3& GetMin() const { return AABoundingBox::min; }
        const glm::vec3& GetMax() const { return AABoundingBox::max; }

    private:
        GameObject* owner;
        glm::mat4 modelMatrix;

        glm::vec3 min;
        glm::vec3 max;
};

enum ObjectTypeId
{
    TYPEID_STATIC     = 0,
    TYPEID_DYNAMIC    = 1,
    TYPEID_PROJECTILE = 2,

    TYPEID_MAX,
};

class GameObject
{
    public:
        explicit GameObject(std::string model, std::string texture);
        virtual ~GameObject() { delete boundingBox; }

        virtual void OnRender();
        //virtual void OnUpdate(const uint32 &) {}

        const glm::mat4& GetModelMatrix() const;

        void SetGuid(uint32 guid);
        uint32 GetGuid() const { return guid; }

        void SetTypeId(ObjectTypeId type) { typeId = type; }
        ObjectTypeId GetTypeId() { return typeId; }

        void EnableBoundingBox();
        AABoundingBox* GetBoundingObject() const { return boundingBox; }

        void SetPosition(const Position& pos);
        Position& GetPosition() { return position; }

        void SetScale(const glm::vec3& scale);
        const glm::vec3& GetScale() { return scale; }

        std::string GetModel() const { return modelName; }
        std::string GetTexture() const { return textureName; }

        virtual bool IsControllable() const { return false; }
        virtual bool IsDynamicObject() const { return false; }

        //float IsTextured() const { return GetTexture() != "placeholder.tga" ? 1.0f : 0.0f; }
        float coll;

        float GetRotationX() const { return rotationX; }
        float GetRotationY() const { return rotationY; }

        virtual void SetRotationX(float rotation) { rotationX = rotation; }
        virtual void SetRotationY(float rotation) { rotationY = rotation; }

    protected:
        void recreateModelMatrix();

        uint32 guid;
        ObjectTypeId typeId;

        std::string modelName;
        std::string textureName;

        glm::vec3 scale;
        glm::vec3 position;

        float rotationX;
        float rotationY;

        AABoundingBox* boundingBox;

        glm::mat4 modelMatrix;
};

#endif
