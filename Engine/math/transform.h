#pragma once

#include "vector.h"

#include <SFML/System/Vector2.hpp>

class Transform
{
public:
    Transform() = default;
    Transform(sf::Vector2f position_, float rotationDegrees_):
        _translation{ position_ },
        _rotationDegrees{ rotationDegrees_ }
    {
    }

    // Applies translation and rotation.
    sf::Vector2f TransformPosition(sf::Vector2f position_) const;
    sf::Vector2f InverseTransformPosition(sf::Vector2f position_) const;

    // Applies only rotations (vectors have no position).
    sf::Vector2f TransformVector(sf::Vector2f vector_) const;
    sf::Vector2f InverseTransformVector(sf::Vector2f vector_) const;
    
private:
    sf::Vector2f _translation = ZERO_VECTOR_F;
    float _rotationDegrees = 0.f;

};
