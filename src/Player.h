#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "MovableObject.h"

class Player : public DynamicObject
{
public:
    Player() : DynamicObject() {}
    ~Player() {}

    virtual OnUpdate(const uint32 &) override;
private:

};


#endif // PLAYER_H_INCLUDED
