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
            ob.SetOrientation(ob.GetAngle(player)); \
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

#define ADDSTATICOBJECT(x, a,b,c,d,e) \
    ob = new GameObject(a, b);\
    ob->SETPOSITION(c, d,e);\
    ob->SetScale(glm::vec3(1));\
    if (x) ob->EnableBoundingBox();\
    RegisterObject(ob);\

#define ADDGRASS(c,d) ADDSTATICOBJECT(false, "grass.obj","grass.tga",c,d,0.15f) \
    ob->SetScale(glm::vec3(sRandom->Float(0.46f, 2.0f))); \
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f)); \

#define ADDSHROOMS(c,d) ADDSTATICOBJECT(false, "shrooms.obj", sRandom->Int(0, 1) ? "shrooms1.tga" : "shrooms2.tga" ,c,d,0.35f) \
    ob->SetScale(glm::vec3(sRandom->Float(1.0f, 2.0f))); \
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f)); \

#define ADDROCK(c, d) ADDSTATICOBJECT(true, "rock.obj", "rock.tga", c, d, 0.25f) \
    ob->SetScale(glm::vec3(sRandom->Float(0.4f, 1.8f))); \
    ob->SetOrientation(sRandom->Float(0.0f, 360.0f)); \

#endif
