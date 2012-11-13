#include "Config.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "Common.h"

void Config::LoadFile(std::string fileName)
{
    try
    {
        std::ifstream file(fileName);
        if (!file.is_open())
            throw std::exception();

        const uint32 LINE_SIZE = 50;
        char line[LINE_SIZE];

        while(!file.eof())
        {
            file.getline(line, LINE_SIZE, ';');
            parseLine(std::string(line));
        }
    }
    catch (std::exception& e)
    {

    }

    for (auto i = itemsMap.begin(); i != itemsMap.end(); ++i)
        std::cout << i->first << "=" << i->second << std::endl;  

    std::cout << Get<int>("test") << std::endl;
}

void Config::parseLine(std::string& line)
{
    auto end = std::remove_if(line.begin(), line.end(), [](char& c) { return c == ' ' || c == '\n';});
    line.erase(end, line.end());

    if (size_t pos = line.find("="))
    {
        if (pos == std::string::npos)
            throw std::exception();

        itemsMap[line.substr(0, pos)] = line.substr(pos+1);
    }
}

template<class T>
T Config::Get(std::string key)
{
    T out;
    auto i = itemsMap.find(key);

    std::stringstream istr(i != itemsMap.end() ? i->second : "NULL");
    if (istr >> out)
        return out;

    throw std::exception();
}
