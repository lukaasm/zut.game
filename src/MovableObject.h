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
    MOVE_ROTATE_UP    = 0x100,
    MOVE_ROTATE_DOWN  = 0x200,
};

class Player : public GameObject
{
    friend class Camera;
public:
    Player();

    void Move(MoveType, float);
            
    void AddMoveType(MoveType);
    void ClearMoveType(MoveType);
    void OnUpdate(const uint32 diff) override;
private:
    MoveType moveFlags;

    glm::vec3 lookAt;

    glm::vec3 right;
    glm::vec3 up;
};

#endif