#ifndef H_PLAYER
#define H_PLAYER

#include "GameObject.h"

enum MoveTypes
{
    MOVE_TYPE_FORWARD       = 0,
    MOVE_TYPE_BACKWARD      = 1,
    MOVE_TYPE_ROTATE_LEFT   = 2,
    MOVE_TYPE_ROTATE_RIGHT  = 3,
    MOVE_TYPE_STRAFE_RIGHT  = 4,
    MOVE_TYPE_STRAFE_LEFT   = 5,
    MOVE_TYPE_UPWARD        = 6,
    MOVE_TYPE_DOWNWARD      = 7,
    MOVE_TYPE_NONE          = 8,

    MAX_MOVE_TYPES          = 8
};

enum MoveFlags
{
    MOVE_FLAG_NONE          = 0x000,
    MOVE_FLAG_FORWARD       = 0x001,
    MOVE_FLAG_BACKWARD      = 0x002,
    MOVE_FLAG_ROTATE_LEFT   = 0x004,
    MOVE_FLAG_ROTATE_RIGHT  = 0x008,
    MOVE_FLAG_STRAFE_RIGHT  = 0x010,
    MOVE_FLAG_STRAFE_LEFT   = 0x020,
    MOVE_FLAG_UPWARD        = 0x040,
    MOVE_FLAG_DOWNWARD      = 0x080,
};

struct MoveInfo
{
    MoveFlags apply;
    MoveFlags remove;

    float speed;
};

const MoveInfo moveInfos[] =
{
    { MOVE_FLAG_FORWARD,        MOVE_FLAG_BACKWARD,     1.0f },     // MOVE_TYPE_FORWARD
    { MOVE_FLAG_BACKWARD,       MOVE_FLAG_FORWARD,      0.6f },     // MOVE_TYPE_BACKWARD
    { MOVE_FLAG_STRAFE_LEFT,    MOVE_FLAG_STRAFE_RIGHT, 1.0f },     // MOVE_TYPE_STRAFE_LEFT
    { MOVE_FLAG_STRAFE_RIGHT,   MOVE_FLAG_STRAFE_LEFT,  1.0f },     // MOVE_TYPE_STRAFE_RIGHT
    { MOVE_FLAG_ROTATE_LEFT,    MOVE_FLAG_ROTATE_RIGHT, 0.5f },     // MOVE_TYPE_ROTATE_LEFT
    { MOVE_FLAG_ROTATE_RIGHT,   MOVE_FLAG_ROTATE_LEFT,  0.5f },     // MOVE_TYPE_ROTATE_RIGHT
    { MOVE_FLAG_UPWARD,         MOVE_FLAG_DOWNWARD,     1.0f },     // MOVE_TYPE_UPWARD
    { MOVE_FLAG_DOWNWARD,       MOVE_FLAG_UPWARD,       1.0f },     // MOVE_TYPE_DOWNWARD
    { MOVE_FLAG_NONE,           MOVE_FLAG_NONE,         0.0f }      // MOVE_TYPE_NONE/MAX_MOVE_TYPES
};

class Player : public GameObject
{
    friend class Camera;
    public:

        Player();

        void Move(MoveFlags, float);

        void AddMoveType(MoveInfo);
        void ClearMoveType(MoveFlags);

        bool IsControllable() const { return true; }

        void OnUpdate(const uint32 diff) override;

    private:
        MoveFlags moveFlags;

        glm::vec3 lookAt;

        glm::vec3 right;
        glm::vec3 up;
};

#endif
