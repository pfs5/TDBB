#pragma once

#include "engine/engineaccess.h"
#include "engine/enginemodule.h"

class EntityBase;

// ptodo - remove?
class EngineModule_Entity : public EngineModule<EngineModule_Entity>, public IEngineEntityAccess
{
    using Super = EngineModule<EngineModule_Entity>;

public:
    using Super::Super;

    // Engine module
    virtual void Init() override;
    virtual void Shutdown() override;
    virtual void Update(float deltaSeconds_) override;
    virtual void Draw(sf::RenderTarget& renderTarget_) override;
    virtual void DrawEditor() override;
    
    // IEngineEntityAccess
    virtual void SetupEntity(EntityBase& entity_, sf::Vector2f position_) override;

private:
    // ptodo - smart obj ptr + memory pool
    std::vector<EntityBase*> _entities;
};