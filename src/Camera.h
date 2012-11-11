/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_CAMERA
#define H_CAMERA

#include <glm/glm.hpp>

#include "Common.h"
//#include "Frustum.h"

class RenderDevice;
class Player;

class Camera
{
    public:
        Camera();

        void OnResize(int32, int32);
        void OnUpdate(const uint32);

        void LookAt();
        void SetOwner(Player* p) { owner = p; }

        glm::mat4 GetProjMatrix() const { return projMatrix; }
        glm::mat4 GetViewMatrix() const { return viewMatrix; }
        
        //Frustum& GetFrustum() { return frustum; }

    private:
        glm::mat4 viewMatrix;
        glm::mat4 projMatrix;

        Player* owner;
        //Frustum frustum;
};

#endif
