#pragma once

#include <SFML/System/Vector2.hpp>
#include <cstdint>

namespace sf
{
    class RenderWindow;
}

// Interface used to expose engine functionality to engine modules.
class IModuleEngineAccess
{
public:
    virtual bool IsSimulationActive() const = 0;

    virtual uint64_t GetEditorTimeMilliseconds() = 0;

    virtual sf::Vector2i GetGameWindowPosition() const = 0;
    virtual const sf::RenderWindow* GetGameWindow() const = 0;
};