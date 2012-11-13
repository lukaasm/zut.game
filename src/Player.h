#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "DynamicObject.h"

class Player : public DynamicObject
{
public:
    Player() : DynamicObject() {}
    ~Player() {}

    virtual void OnUpdate(const uint32 &) override;
private:

};


#endif // PLAYER_H_INCLUDED
