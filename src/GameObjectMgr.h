/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_GOBJECTMGR
#define H_GOBJECTMGR

#include <map>

#include "Common.h"
#include "GameObject.h"
#include "Singleton.h"

//class GameObject;
typedef std::map<uint32, GameObject*> GameObjectsMap;

class GameObjectMgr
{
    INIT_SINGLETON(GameObjectMgr)
    
    public:
        uint32 GenerateNewGUID() { return ++guidCounter; }
        
        GameObject* CreateGameObject()
        {
            uint32 guid = GenerateNewGUID();
            GameObject* object = new GameObject();

            object->SetGuid(guid);

            gameObjectsMap[guid] = object;
            return object;
        }

        GameObjectsMap& GetGameObjects() { return gameObjectsMap; }

    private:
        GameObjectsMap gameObjectsMap;

        uint32 guidCounter;

};

#define sGameObjectMgr Singleton<GameObjectMgr>::Instance()
#endif