#include "enginemodule_debug.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "util/arrayutils.h"

void EngineModule_Debug::Init()
{
    Super::Init();

    EngineDebugAccess::Open(*this);
}

void EngineModule_Debug::Shutdown()
{
    EngineDebugAccess::Close(*this);
    
    Super::Shutdown();
}

void EngineModule_Debug::Update(float deltaSeconds_)
{
    Super::Update(deltaSeconds_);

    for (int32_t i = _debugLines.size() - 1; i >= 0; --i)
    {
        Line& line = _debugLines[i];
        
        if (line.DurationSeconds < 0.f)
        {
            RemoveAtSwap(_debugLines, i);
        }
        else
        {
            line.DurationSeconds -= deltaSeconds_;
        }
    }

    for (int32_t i = _debugPoints.size() - 1; i >= 0; --i)
    {
        Point& point = _debugPoints[i];
        
        if (point.DurationSeconds < 0.f)
        {
            RemoveAtSwap(_debugPoints, i);
        }
        else
        {
            point.DurationSeconds -= deltaSeconds_;
        }
    }
}

void EngineModule_Debug::Draw(sf::RenderTarget& renderTarget_)
{
    Super::Draw(renderTarget_);

    for (Line& line : _debugLines)
    {
        const sf::Vector2f direction = line.End - line.Start;
        const float length = Math::VectorSize(direction);
        static constexpr float THICKNESS = 1.f;
        
        sf::RectangleShape shape { sf::Vector2f{ length, THICKNESS} };
        shape.setOrigin(sf::Vector2f{ 0.f, THICKNESS * 0.5f });
        shape.setPosition(line.Start);
        shape.setRotation(Math::VectorAngleDeg(RIGHT_VECTOR_F, direction));
        shape.setOutlineColor(sf::Color::Transparent);
        shape.setFillColor(sf::Color::Green);
        renderTarget_.draw(shape);
    }

    for (Point& point : _debugPoints)
    {
        sf::CircleShape shape { point.Radius };
        shape.setOrigin(sf::Vector2f{ point.Radius, point.Radius });
        shape.setPosition(point.Position);
        shape.setOutlineColor(sf::Color::Transparent);
        shape.setFillColor(sf::Color::Green);
        renderTarget_.draw(shape);
    }
}

void EngineModule_Debug::DrawDebugLine(sf::Vector2f start_, sf::Vector2f end_, float durationSeconds_/* = 0.f*/)
{
    _debugLines.emplace_back(durationSeconds_, start_, end_);
}

void EngineModule_Debug::DrawDebugPoint(sf::Vector2f position_, float radius_, float durationSeconds_/* = 0.f*/)
{
    _debugPoints.emplace_back(durationSeconds_, position_, radius_);
}
