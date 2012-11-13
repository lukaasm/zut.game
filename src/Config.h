#ifndef H_CONFIG
#define H_CONFIG

#include <hash_map>
#include <string>

#include "Singleton.h"

typedef std::hash_map<std::string, std::string> ConfigItemsMap;

class Config
{
    SINGLETON(Config)
    public:
        void LoadFile(std::string);

        template<class T>
        T Get(std::string key);

    private:
        void parseLine(std::string&);

        ConfigItemsMap itemsMap;
};

#define sConfig Singleton<Config>::Instance()
#endif
