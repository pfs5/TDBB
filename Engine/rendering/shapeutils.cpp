#include "shapeutils.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "math/vector.h"

sf::RectangleShape Shapes::MakeLine(sf::Vector2f direction_, float thickness_)
{
    const float length = Math::VectorSize(direction_);
    
    sf::RectangleShape lineRight { sf::Vector2f{length , thickness_ } };
    lineRight.setOrigin(sf::Vector2f{ 0.f, thickness_});
    lineRight.setRotation(Math::VectorAngleDeg(RIGHT_VECTOR_F, direction_));

    return lineRight;
}
