/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_CAMERA
#define H_CAMERA

#include <glm/glm.hpp>

#include "Common.h"
//#include "Frustum.h"

class RenderDevice;

enum MoveType
{
    MOVE_NONE         = 0x000,
    MOVE_FORWARD      = 0x001,
    MOVE_BACKWARD     = 0x002,
    MOVE_ROTATE_LEFT  = 0x004,
    MOVE_ROTATE_RIGHT = 0x008,
    MOVE_STRAFE_RIGHT = 0x010,
    MOVE_STRAFE_LEFT  = 0x020,
    MOVE_UPWARD       = 0x040,
    MOVE_DOWNWARD     = 0x080,
    MOVE_ROTATE_UP    = 0x100,
    MOVE_ROTATE_DOWN  = 0x200,
};

class Camera
{
    public:
        Camera();

        void AddMoveType(MoveType);
        void ClearMoveType(MoveType);

        void OnResize(int32, int32);
        void OnUpdate(const uint32);

        void LookAt();

        void Move(MoveType, float);

        glm::mat4 GetProjMatrix() const { return projMatrix; }
        glm::mat4 GetViewMatrix() const { return viewMatrix; }
        
        //Frustum& GetFrustum() { return frustum; }

    private:
        glm::mat4 viewMatrix;
        glm::mat4 projMatrix;

        glm::vec3 lookAt;
        glm::vec3 position;

        glm::vec3 right;
        glm::vec3 up;

        //Frustum frustum;
        MoveType moveFlags;
};

#endif
