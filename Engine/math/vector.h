#pragma once

#include "mathutils.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>

static inline const sf::Vector2f ZERO_VECTOR_F { 0.f, 0.f };
static inline const sf::Vector2i ZERO_VECTOR_I { 0, 0 };
static inline const sf::Vector2f ONE_VECTOR_F { 1.f, 1.f };

static inline const sf::Vector2f RIGHT_VECTOR_F { 1.f, 0.f };
static inline const sf::Vector2f UP_VECTOR_F    { 0.f, 1.f };

namespace Math
{
    inline float VectorSizeSquared(sf::Vector2f vector_) { return vector_.x * vector_.x + vector_.y * vector_.y; }
    inline float VectorSize(sf::Vector2f vector_) { return std::sqrt(VectorSizeSquared(vector_)); }

    inline float VectorDot(sf::Vector2f a_, sf::Vector2f b_) { return a_.x * b_.x + a_.y * b_.y; }
    inline float VectorCross(sf::Vector2f a_, sf::Vector2f b_) { return a_.x * b_.y - b_.x * a_.y; }
    
    inline sf::Vector2f NormalizeVector(sf::Vector2f vector_) { return vector_ / VectorSize(vector_); }
    inline sf::Vector2f NormalizeVectorSafe(sf::Vector2f vector_)
    {
        if (IsNearlyZero(VectorSizeSquared(vector_))) { return ZERO_VECTOR_F;}
        return NormalizeVector(vector_);
    }

    sf::Vector2f RotateVector(sf::Vector2f vector_, float angleRadians_);
    float VectorAngleDeg(sf::Vector2f a_, sf::Vector2f b_);
    
    sf::Vector2f ProjectPointOntoLine(sf::Vector2f point_, sf::Vector2f lineStart_, sf::Vector2f lineDirection_);
}