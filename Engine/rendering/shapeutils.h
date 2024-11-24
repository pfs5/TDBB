#pragma once
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RectangleShape;
}

namespace Shapes
{
    sf::RectangleShape MakeLine(sf::Vector2f vector_, float thickness_);
    
}
