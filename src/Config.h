#ifndef H_CONFIG
#define H_CONFIG

#ifdef __GNUG__
#include <unordered_map>
#define hash_map unordered_map
#else
#include <hash_map>
#endif

#include <string>
#include <sstream>
#include <algorithm>

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
        void parseLine(std::string);

        ConfigItemsMap itemsMap;
};

template<class T>
T Config::Get(std::string key)
{
    T out;
    auto i = itemsMap.find(key);

    std::stringstream istr(i != itemsMap.end() ? i->second : "-1");
    if (istr >> out)
        return out;

    throw std::exception();
}


#define sConfig Singleton<Config>::Instance()
#endif
