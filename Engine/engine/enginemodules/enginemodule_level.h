#pragma once

#include "engine/engineaccess.h"
#include "engine/enginemodule.h"
#include "engine/level/level.h"

class EngineModule_Level : public EngineModule<EngineModule_Level>, public IEngineLevelAccess
{
    using Super = EngineModule<EngineModule_Level>;

public:
    using Super::Super;
    
    virtual void Init() override;
    virtual void Shutdown() override;

    virtual void Update(float deltaSeconds_) override;
    virtual void Draw(sf::RenderTarget& renderTarget_) override;
    
    virtual void DrawEditor() override;

    virtual void LoadLevel(const char* path_) override;

    void SaveCurrentLevel();

private:
    // There's a level loaded at any time. The default is an empty level.
    Level _currentLevel;
};
