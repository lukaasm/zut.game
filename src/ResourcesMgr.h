/*#################################################
# Copyright (C) 2012 lukaasm <http://lukaasm.eu/>
#################################################*/

#ifndef H_RESOURCESMGR
#define H_RESOURCESMGR

#include "Common.h"
#include "Singleton.h"

class ResourcesMgr
{
    INIT_SINGLETON(ResourcesMgr)

    public:

};

#define sResourcesMgr Singleton<ResourcesMgr>::Instance()
#endif