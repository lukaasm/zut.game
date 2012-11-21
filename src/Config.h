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
        template<class T>
        T GetDefault(std::string key, T dflt);

        template<class T>
        void Set(std::string, T);

    private:
        void parseLine(std::string);

        ConfigItemsMap itemsMap;
};

template<class T>
void Config::Set(std::string name, T key)
{
    std::stringstream istr;
    istr << key;
    istr >> itemsMap[name];
}

template<class T>
T Config::Get(std::string key)
{
    T out;
    auto i = itemsMap.find(key);

    std::stringstream istr(i != itemsMap.end() ? i->second : "-1");
    istr >> out;
    return out;
}

template<class T>
T Config::GetDefault(std::string key, T dflt)
{
    T out;
    auto i = itemsMap.find(key);

    if (i != itemsMap.end())
    {
        std::stringstream istr(i->second);
        istr >> out;
        return out;
    }

    return dflt;
}

#define sConfig Singleton<Config>::Instance()
#endif
