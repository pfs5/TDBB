#pragma once

#include <random>

namespace Math
{
    inline float RandomFloat() { return static_cast<float>(rand()) / RAND_MAX; }
}