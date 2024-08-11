#pragma once

#include <cstdint>

struct ApplicationSetupParams
{
    uint32_t GameWindowSizeX = 0u;
    uint32_t GameWindowSizeY = 0u;

    // ptodo - don't do this like this
    class Level* StartLevel = nullptr;
    
    ApplicationSetupParams& SetGameWindowSize(uint32_t x, uint32_t y) { GameWindowSizeX = x; GameWindowSizeY = y; return *this; }
    template<typename LevelClass>
    ApplicationSetupParams& SetStartLevel() { /*static_assert(std::is_base_of_v<Level, LevelClass>);*/ StartLevel = new LevelClass(); return *this; }
};

class Application
{
public:
    void Run(const ApplicationSetupParams& params_);
};