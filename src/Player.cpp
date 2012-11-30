#include "Player.h"

#include "Input.h"
#include "SceneMgr.h"

void Player::OnUpdate(const uint32 & diff)
{
    if (IsControllable())
    {
        for (Keyboard::KeysMap::const_iterator i = sKeyboard->GetKeysMap().begin(); i != sKeyboard->GetKeysMap().end(); ++i)
        {
            const MoveInfo& info = Keyboard::Key2MoveInfo(i->first);
            if (info.apply == MOVE_FLAG_NONE)
                continue;

            if (i->second)
                AddMoveType(info);
            else
                ClearMoveType(info.apply);
        }
    }

    DynamicObject::OnUpdate(diff);

    position.y = sSceneMgr->GetHeight(position.x, position.z);
}

