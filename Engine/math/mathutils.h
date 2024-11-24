#pragma once

#define M_PI             3.14159265358979323846f
#define M_SMALL_NUMBER   1e-6f

#include <cmath>

namespace Math
{
    inline bool IsNearlyZero(float number_, float tolerance_ = M_SMALL_NUMBER) { return std::abs(number_) < tolerance_; }

    inline float Sign(float number_) { return number_ >= 0 ? 1.f : -1.f; }
    
    inline float RadianToDegree(float valueRadian_) { return valueRadian_ / M_PI * 180.f; }
    inline float DegreeToRadian(float valueDegree_) { return valueDegree_ / 180.f * M_PI; }
}