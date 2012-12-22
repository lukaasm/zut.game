/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_CAMERA
#define H_CAMERA

#include <glm/glm.hpp>

#include "Common.h"
//#include "Frustum.h"

class DynamicObject;

class Camera
{
    public:
        Camera();

        void OnResize(int32, int32);

        virtual void OnUpdate(const uint32&) = 0;
        virtual void LinkTo(DynamicObject*) = 0;

        void LookAt(const Position& pos, const glm::vec3& dir, const glm::vec3& up);

        glm::mat4 GetProjMatrix() const { return projMatrix; }
        glm::mat4 GetViewMatrix() const { return viewMatrix; }

        virtual void SetPosition(Position& pos) { position = pos; }
        glm::vec3& GetPosition() { return position; }

    protected:
        glm::mat4 viewMatrix;
        glm::mat4 projMatrix;

        glm::vec3 position;
};

class FppCamera : public Camera
{
    public:
        FppCamera() : owner(nullptr) {}

        void OnUpdate(const uint32 &) override;

        void LinkTo(DynamicObject* o) override;
        DynamicObject* GetLinkedObject() { return owner; }

        void SetPosition(Position& pos) override;

    private:
        DynamicObject* owner;
};

class TppCamera : public Camera
{
    public:
        TppCamera() : owner(nullptr) {}

        void OnUpdate(const uint32 &) override;

        void LinkTo(DynamicObject* o) override;
        DynamicObject* GetLinkedObject() { return owner; }

        void SetPosition(Position& pos) override;

    private:
        DynamicObject* owner;
};

class EagleEyeCamera : public Camera
{
    public:
        EagleEyeCamera() : owner(nullptr) {}

        void OnUpdate(const uint32 &) override;

        void LinkTo(DynamicObject* o) override;
        DynamicObject* GetLinkedObject() { return owner; }

        void SetPosition(Position& pos) override;

    private:
        DynamicObject* owner;
};

#endif
