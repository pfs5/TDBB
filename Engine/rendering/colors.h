#pragma once

#include <SFML/Graphics/Color.hpp>

inline ImVec4 ToImVec4(const sf::Color& color_)
{
    return ImVec4{
        static_cast<float>(color_.r) / 255.f,
        static_cast<float>(color_.g) / 255.f,
        static_cast<float>(color_.b) / 255.f,
        static_cast<float>(color_.a) / 255.f
    };
}

namespace Colors
{
    static inline const sf::Color LightGreen {0, 255, 0, 255};
    static inline const sf::Color DarkYellow {200, 200, 0, 255};
}
