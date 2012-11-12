#ifndef H_PLAYER
#define H_PLAYER

#include "GameObject.h"

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
};

struct MoveFlag
{
    MoveType apply;
    MoveType remove;

    float speed;
};

#define MAX_MOVE_FLAGS 8

const MoveFlag moveflags[] =
{
    { MOVE_FORWARD, MOVE_BACKWARD, 1.0f },
    { MOVE_BACKWARD, MOVE_FORWARD, 0.6f },
    { MOVE_STRAFE_LEFT, MOVE_STRAFE_RIGHT, 1.0f },
    { MOVE_STRAFE_RIGHT, MOVE_STRAFE_LEFT, 1.0f },
    { MOVE_ROTATE_LEFT, MOVE_ROTATE_RIGHT, 0.5f },
    { MOVE_ROTATE_RIGHT, MOVE_ROTATE_LEFT, 0.5f },
    { MOVE_UPWARD, MOVE_DOWNWARD, 1.0f },
    { MOVE_DOWNWARD, MOVE_UPWARD, 1.0f },
    { MOVE_NONE, MOVE_NONE, 0.0f }
};

class Player : public GameObject
{
    friend class Camera;
    public:

        Player();

        void Move(MoveType, float);
            
        void AddMoveType(MoveFlag);
        void ClearMoveType(MoveType);

        bool IsControllable() const { return true; }

        void OnUpdate(const uint32 diff) override;

    private:
        MoveType moveFlags;

        glm::vec3 lookAt;

        glm::vec3 right;
        glm::vec3 up;
};

#endif
