#include "Config.h"

#include <fstream>
#include <iostream>

#include "Common.h"
#include "Exception.h"

void Config::LoadFile(std::string fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
        throw Exception("can NOT open config file.");

    const uint32 LINE_SIZE = 50;
    char line[LINE_SIZE];

    while (!file.eof())
    {
        file.getline(line, LINE_SIZE, ';');
        parseLine(line);
    }
}

void Config::parseLine(std::string line)
{
    auto end = std::remove_if(line.begin(), line.end(), [](char& c) { return c == ' ' || c == '\n';});
    line.erase(end, line.end());

    auto pos = line.find("=");
    if (pos == std::string::npos)
        return;

    itemsMap[line.substr(0, pos)] = line.substr(pos+1);
}
