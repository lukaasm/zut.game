#ifndef H_RANDOM
#define H_RANDOM

#include <random>

#include "Singleton.h"

class Random
{
    SINGLETON(Random)

    public:
        float Float(float min, float max)
        {
            std::uniform_real_distribution<> dis(min, max);
            return dis(gen);
        }

        int Int(int min, int max)
        {
            std::uniform_int_distribution<> dis(min, max);
            return dis(gen);
        }

    private:
        std::random_device rd;
        std::mt19937 gen;
};

#define sRandom Singleton<Random>::Instance()
#endif
