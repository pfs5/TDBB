#include "transform.h"

#include "mathutils.h"
#include "vector.h"

sf::Vector2f Transform::TransformPosition(sf::Vector2f position_) const
{
    return TransformVector(position_) + _translation;
}

sf::Vector2f Transform::InverseTransformPosition(sf::Vector2f position_) const
{
    return InverseTransformVector(position_ - _translation);
}

sf::Vector2f Transform::TransformVector(sf::Vector2f vector_) const
{
    return Math::RotateVector(vector_, Math::DegreeToRadian(_rotationDegrees));
}

sf::Vector2f Transform::InverseTransformVector(sf::Vector2f vector_) const
{
    return Math::RotateVector(vector_, Math::DegreeToRadian(-_rotationDegrees));    
}

