#pragma once
#include "engine/level/level.h"

class Level_Showcase : public Level
{
public:
    virtual void SetupLevel() override;
    virtual void StartLevel() override;
    virtual void StopLevel() override;
};
