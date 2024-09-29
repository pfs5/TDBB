#pragma once

#include <SFML/System/Vector2.hpp>

struct Bounds
{
    sf::Vector2f Min;
    sf::Vector2f Max;

    Bounds(sf::Vector2f min_, sf::Vector2f max_):
        Min{ min_ },
        Max{ max_ }
    {
        
    }

    static Bounds FromCenterSize(sf::Vector2f center_, sf::Vector2f size_)
    {
        const sf::Vector2f halfsize = size_ * 0.5f;
        return Bounds{ center_ - halfsize, center_ + halfsize };
    }
    
    bool IsPointInside(sf::Vector2f point_) const
    {
        return point_.x >= Min.x && point_.y >= Min.y &&
            point_.x <= Max.x && point_.y <= Max.y;
    }
    
};
