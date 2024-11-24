#pragma once

#include "engine/engineaccess.h"
#include "engine/enginemodule.h"
#include "math/vector.h"

class EngineModule_Debug : public EngineModule<EngineModule_Debug>, public IEngineDebugAccess
{
    using Super = EngineModule<EngineModule_Debug>;

public:
    using Super::Super;

    // EngineModule
    virtual void Init() final;
    virtual void Shutdown() final;
    virtual void Update(float deltaSeconds_) final;
    virtual void Draw(sf::RenderTarget& renderTarget_) final;
    
    // IEngineDebugAccess
    virtual void DrawDebugLine(sf::Vector2f start_, sf::Vector2f end_, float durationSeconds_ = 0.f) final;
    virtual void DrawDebugPoint(sf::Vector2f position_, float radius_, float durationSeconds_ = 0.f) final;

private:
    struct DebugShape
    {
        float DurationSeconds = 0.f;
    };
    
    struct Line : public DebugShape
    {
        sf::Vector2f Start = ZERO_VECTOR_F;
        sf::Vector2f End = ZERO_VECTOR_F;

        Line(float duration_, sf::Vector2f start_, sf::Vector2f end_):
            DebugShape{ duration_ }, Start{ start_ }, End{ end_ }
        {
            
        }
    };

    struct Point : public DebugShape
    {
        sf::Vector2f Position = ZERO_VECTOR_F;
        float Radius = 0.f;

        Point(float duration_, sf::Vector2f position_, float radius_):
            DebugShape{ duration_ }, Position{ position_ }, Radius{ radius_ }
        {
            
        }
    };

private:
    std::vector<Line> _debugLines;
    std::vector<Point> _debugPoints;
    
};
