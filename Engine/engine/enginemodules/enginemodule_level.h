#pragma once

#include "engine/engineaccess.h"
#include "engine/enginemodule.h"
#include "engine/level/level.h"

class EntityBase;

class EngineModule_Level : public EngineModule<EngineModule_Level>, public IEngineLevelAccess
{
    using Super = EngineModule<EngineModule_Level>;

public:
    using Super::Super;

    virtual void Init() override;
    virtual void OnEngineStarted() override;
    virtual void Shutdown() override;

    virtual void Update(float deltaSeconds_) override;
    virtual void Draw(sf::RenderTarget& renderTarget_) override;

#ifdef _EDITOR
    virtual void DrawEditor() override;
#endif //_EDITOR

    virtual void LoadLevel(const char* path_) override;
    virtual EntityAddedDelegate& EntityAdded() override { return _entityAdded; }
    virtual CurrentLevelChangedDelegate& CurrentLevelChanged() override { return _currentLevelChanged; }
    
    void SaveCurrentLevel() const;

private:
    // There's a level loaded at any time. The default is an empty level.
    Level _currentLevel;

    EntityAddedDelegate _entityAdded;
    CurrentLevelChangedDelegate _currentLevelChanged;
    
private:
    void OnEntityAddedToCurrentLevel(EntityBase& entity_);
};
