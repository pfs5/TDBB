#pragma once

class Level
{
public:
    // Use this function to instantiate entities.
    virtual void SetupLevel() {}
    virtual void StartLevel() {}
    virtual void StopLevel() {}
};