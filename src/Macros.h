#ifndef H_MACROS
#define H_MACROS

#define SETPOSITION2(a,b,c,d) SetPosition(Position(a, GetHeight(a, b, d) - c, b));
#define SETPOSITION(a,b,c) SETPOSITION2(a,b,c,ob)

#define ENEMYSCRIPT    ((DynamicObject*)(ob))->scripts["OnUpdate"].push_back( \
    [](DynamicObject& ob) \
        { \
        DynamicObject* player = sSceneMgr->GetPlayer(); \
        float dist = ob.GetDistance(player); \
        if (dist < 0.5f || dist > 4.6f) \
        ob.ClearMoveType(MOVE_FLAG_FORWARD); \
            else if (dist < 4.5f) \
            { \
            ob.SetRotationY(ob.GetAngle(player)); \
            ob.AddMoveType(moveInfos[MOVE_TYPE_FORWARD]); \
            ob.GetPosition().y = sSceneMgr->GetHeight(&ob); \
            } \
        }); \
        \
        ((DynamicObject*)(ob))->scripts["OnCollision"].push_back( \
        [](DynamicObject& ob) \
        { \
        sSceneMgr->UnregisterObject(&ob); \
        }); \

#endif
