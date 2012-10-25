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
    SINGLETON(GameObjectMgr)
    
    public:
        uint32 GenerateNewGUID() { return ++guidCounter; }

        GameObjectsMap& GetGameObjects() { return gameObjectsMap; }

    private:
        GameObjectsMap gameObjectsMap;

        uint32 guidCounter;

};

#define sGameObjectMgr Singleton<GameObjectMgr>::Instance()
#endif