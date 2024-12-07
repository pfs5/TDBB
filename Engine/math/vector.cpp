#include "vector.h"

#include <algorithm>

sf::Vector2f Math::RotateVector(sf::Vector2f vector_, float angleRadians_)
{
    // Standard linear transformation
    return sf::Vector2f
    {
        std::cos(angleRadians_) * vector_.x - std::sin(angleRadians_) * vector_.y,
        std::sin(angleRadians_) * vector_.x + std::cos(angleRadians_) * vector_.y
    };
}

float Math::VectorAngleDeg(sf::Vector2f a_, sf::Vector2f b_)
{
    const float aSize = VectorSize(a_);
    if (IsNearlyZero(aSize))
    {
        return 0.f;
    }

    const float bSize = VectorSize(b_);
    if (IsNearlyZero(bSize))
    {
        return 0.f;        
    }

    const float dot = VectorDot(a_, b_);
    const float dotNormalized = std::clamp(dot / (aSize * bSize), -1.f, 1.f);

    return RadianToDegree(std::acos(dotNormalized)) * Sign(VectorCross(a_, b_));
}

sf::Vector2f Math::ProjectPointOntoLine(sf::Vector2f point_, sf::Vector2f lineStart_, sf::Vector2f lineDirection_)
{
    // 1 - Move point to line's local system
    const sf::Vector2f pointLocal = point_ - lineStart_;

    // 2 - Project point-vector to line-vector
    const sf::Vector2f lineDirectionNormalized = NormalizeVector(lineDirection_);
    const sf::Vector2f projectionVector = lineDirectionNormalized * VectorDot(pointLocal, lineDirectionNormalized);

    return lineStart_ + projectionVector;
}
