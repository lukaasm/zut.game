#ifndef H_MACROS
#define H_MACROS

#define SETPOSITION2(a,b,c,d) SetPosition(Position(a, GetHeight(a, b, d) - c, b));
#define SETPOSITION(a,b,c) SETPOSITION2(a,b,c,ob)

#define ADDSTATICOBJECT(x, a,b,c,d,e) \
    ob = new GameObject(a, b);\
    ob->SETPOSITION(c, d,e);\
    ob->SetScale(glm::vec3(1));\
    if (x) ob->EnableBoundingBox();\
    RegisterObject(ob);\

#define ADDPALM(c, d, e) \
    ADDSTATICOBJECT(false, "palm.obj", "palm.tga", c, d, e) \
    ob->SetScale(glm::vec3(sRandom->Float(0.8f, 1.80f))); \
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f)); \

#define ADDGRASS(c,d) ADDSTATICOBJECT(false, "grass.obj","grass.tga",c,d,0.15f) \
    ob->SetScale(glm::vec3(sRandom->Float(0.86f, 1.8f))); \
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f)); \

#define ADDSHROOMS(c,d) ADDSTATICOBJECT(false, "shrooms.obj", sRandom->Int(0, 1) ? "shrooms1.tga" : "shrooms2.tga" ,c,d,0.35f) \
    ob->SetScale(glm::vec3(sRandom->Float(1.0f, 2.0f))); \
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f)); \

#define ADDROCK(c, d) ADDSTATICOBJECT(true, "rock.obj", "rock.tga", c, d, 0.25f) \
    ob->SetScale(glm::vec3(sRandom->Float(0.4f, 1.8f))); \
    //ob->SetOrientation(sRandom->Float(0.0f, 360.0f)); \

#define ADDENEMY(c, d) \
    { \
    DynamicObject* ob = new DynamicObject("boid.obj", "boid.tga"); \
    ob->SetScale(glm::vec3(0.20f)); \
    ob->SETPOSITION(c, d, 0.20f); \
    ob->EnableBoundingBox(); \
    ob->scripts["OnUpdate"].push_back( \
    [](DynamicObject& ob) \
        { \
        if (ob.GetHealth() <= 0) { \
        sSceneMgr->UnregisterObject(&ob); return; } \
        DynamicObject* player = sSceneMgr->GetPlayer(); \
        float dist = ob.GetDistance(player); \
        if (dist < 0.35f) {\
        player->DamageTaken(&ob, 10); \
        ob.ClearMoveType(MOVE_FLAG_FORWARD); \
        ob.GetPosition().y = sSceneMgr->GetHeight(&ob); \
        } else if (dist > 4.6f) { \
            ob.ClearMoveType(MOVE_FLAG_FORWARD); \
            ob.GetPosition().y = sSceneMgr->GetHeight(&ob) - 0.20f; \
        } else if (dist < 4.5f) \
            { \
            if (ob.GetGuid() == 2 && dist < 2.5f && ob.timers[4].Passed()) \
            { \
                ob.timers[4].Start(100000); \
                sSceneMgr->SetGameState(GAME_INPROGRESS_TUT1); \
            } \
            ob.SetOrientation(ob.GetAngle(player)); \
            ob.AddMoveType(moveInfos[MOVE_TYPE_FORWARD]); \
            ob.GetPosition().y = sSceneMgr->GetHeight(&ob) + abs(sin(ob.timers[0].Elapsed()/256.0f))*0.3f; \
            } \
        }); \
        \
        ob->scripts["OnCollision"].push_back( \
        [](DynamicObject& ob) \
        { \
            ob.DamageTaken(&ob, 100); \
        }); \
    RegisterObject(ob);\
    } \

#define ADDCOIN(c,d) \
    { \
    DynamicObject* ob = new DynamicObject("coin.obj", "coin.tga"); \
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f)); \
    ob->SetScale(glm::vec3(0.20f)); \
    ob->SETPOSITION(c, d, -0.20f); \
    ob->SetTypeId(TYPEID_COIN); \
    ob->EnableBoundingBox(); \
    ob->AddMoveType(moveInfos[MOVE_TYPE_ROTATE_LEFT]); \
    ob->scripts["OnCollision"].push_back( \
        [](DynamicObject& ob) \
        { \
            sSceneMgr->UnregisterObject(&ob); \
        }); \
    RegisterObject(ob);\
    } \

#endif
