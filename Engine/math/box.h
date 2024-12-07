#pragma once

#include "vector.h"

#include <SFML/System/Vector2.hpp>

#include "transform.h"

struct Circle
{
    sf::Vector2f Center;
    float Radius;

    Circle(sf::Vector2f center_, float radius_):
        Center{ center_ },
        Radius{ radius_ }
    {
    }

    bool IsPointInside(sf::Vector2f point_) const
    {
        return Math::VectorSizeSquared(point_ - Center) <= Radius * Radius;
    }
};

struct Box
{
    Transform CenterTransform;
    sf::Vector2f Size = ZERO_VECTOR_F;

    Box(sf::Vector2f center_, sf::Vector2f size_, float rotation_ = 0.f):
        CenterTransform{ center_, rotation_ },
        Size{ size_ }
    {
    }

    static Box FromMinMax(sf::Vector2f min_, sf::Vector2f max_)
    {
        const sf::Vector2f center { (min_ + max_) * 0.5f };
        const sf::Vector2f size { max_ - min_ };
        return Box{ center, size };
    }
    
    bool IsPointInside(sf::Vector2f point_) const
    {
        const sf::Vector2f pointLocal = CenterTransform.InverseTransformPosition(point_);
        const sf::Vector2f halfSize = Size * 0.5f;
        return std::abs(pointLocal.x) <= halfSize.x && std::abs(pointLocal.y) <= halfSize.y;
    }
    
};

struct BoxCircleBounds
{
    enum class EType : uint8_t
    {
        Box,
        Circle
    } Type;

    union
    {
        Circle CircleData;
        Box BoxData;
    };

    bool IsPointInside(sf::Vector2f point_) const
    {
        switch(Type)
        {
            case EType::Box:
            {
                return BoxData.IsPointInside(point_);
            }
            case EType::Circle:
            {
                return CircleData.IsPointInside(point_);
            }
        }

        return false;
    }

    static BoxCircleBounds MakeCircle(sf::Vector2f center_, float radius_)
    {
        BoxCircleBounds circleBounds;
        circleBounds.Type = EType::Circle;
        circleBounds.CircleData = Circle{ center_, radius_ };
        return circleBounds;
    }

    static BoxCircleBounds MakeBox(sf::Vector2f center_, sf::Vector2f size_, float rotation_ = 0.f)
    {
        BoxCircleBounds boxBounds;
        boxBounds.Type = EType::Box;
        boxBounds.BoxData = Box{ center_, size_, rotation_ };
        return boxBounds;
    }

    static BoxCircleBounds MakeBoxFromMinMax(sf::Vector2f min_, sf::Vector2f max_)
    {
        BoxCircleBounds boxBounds;
        boxBounds.Type = EType::Circle;
        boxBounds.BoxData = Box::FromMinMax(min_, max_);
        return boxBounds;
    }

private:
    BoxCircleBounds() {};
};