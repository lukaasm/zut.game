#ifndef H_RANDOM
#define H_RANDOM

#include <random>

#include "Singleton.h"

namespace Random
{
    float Float(float min, float max)
    {
        std::random_device rd;
        std::mt19937 gen(rd);
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    int Int(int min, int max)
    {
        std::random_device rd;
        std::mt19937 gen(rd);
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }
};

#endif
