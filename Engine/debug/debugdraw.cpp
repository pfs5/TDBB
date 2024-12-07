#include "debugdraw.h"

#include "engine/engineaccess.h"

void DrawDebugLine(sf::Vector2f start_, sf::Vector2f end_, float duration_/* = 0.f*/)
{
    EngineDebugAccess::Get().DrawDebugLine(start_, end_, duration_);
}

void DrawDebugPoint(sf::Vector2f position_, float radius_, float duration_/* = 0.f*/)
{
    EngineDebugAccess::Get().DrawDebugPoint(position_, radius_, duration_);
}
