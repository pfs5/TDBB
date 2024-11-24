#pragma once

#include <SFML/Window/Mouse.hpp>

#include "entity/entitymemory.h"
#include "errorhandling/assert.h"
#include "events/delegate.h"
#include "util/proxy.h"

#include "SFML/System/Vector2.hpp"

// ptodo - this should be moved to separate files

// Use these interfaces and proxies to access engine features.

class EntityBase;

// Level access
class IEngineLevelAccess
{
public:
    using EntityAddedDelegate = Delegate<EntityBase& /* entity_ */>;
    using CurrentLevelChangedDelegate = Delegate<>;
    
    virtual void LoadLevel(const char* path_) = 0;
    virtual EntityAddedDelegate& EntityAdded() = 0;
    virtual CurrentLevelChangedDelegate& CurrentLevelChanged() = 0;
};

struct EngineLevelAccess : public Proxy<IEngineLevelAccess>
{
    EngineLevelAccess();
};

// Entity access
class IEngineEntityAccess
{
public:
    template <typename EntityType>
    EntityType* SpawnEntity(const char* name_, sf::Vector2f position_)
    {
        // static_assert(std::is_base_of_v<EntityBase, EntityType>);
        static_assert(true);
        
        EntityType* entity = EntityMemory::InstantiateEntityObject<EntityType>();
        ensure(entity != nullptr);

        entity->SetEntityName(name_);
        
        SetupEntity(*entity, position_);
        entity->EntityCreated();
        
        return entity;
    }

protected:
    virtual void SetupEntity(EntityBase& entity_, sf::Vector2f position_) = 0;
};

struct EngineEntityAccess : public Proxy<IEngineEntityAccess>
{
    EngineEntityAccess();
};

// ptodo - very simple interface for now.
class IEngineLogAccess
{
public:
    virtual void Log(const char* string_) = 0;
    virtual void LogKey(uint32_t key_, const char* string_) = 0;
};

struct EngineLogAccess : public Proxy<IEngineLogAccess>
{
    EngineLogAccess();
};

// Inputs
class IEngineInputsAccess
{
public:
    virtual sf::Vector2i GetMousePosition() const = 0;
    virtual bool IsMouseButtonDown(sf::Mouse::Button mouseButton_) const = 0;
    virtual bool MouseButtonPressed(sf::Mouse::Button mouseButton_) const = 0;
    virtual bool MouseButtonReleased(sf::Mouse::Button mouseButton_) const = 0;
};

struct EngineInputsAccess : public Proxy<IEngineInputsAccess>
{
    EngineInputsAccess();
};

// Settings
class IEngineSettingsAccess
{
public:
    struct RenderSettings
    {
        int AntialiasingLevel = 8;
    };

    virtual const RenderSettings& GetRenderSettings() const = 0;
};

struct EngineSettingsAccess : public Proxy<IEngineSettingsAccess>
{
    EngineSettingsAccess();
};

// Debug
class IEngineDebugAccess
{
public:
    virtual void DrawDebugLine(sf::Vector2f start_, sf::Vector2f end_, float durationSeconds_ = 0.f) = 0;
    virtual void DrawDebugPoint(sf::Vector2f position_, float radius_, float durationSeconds_ = 0.f) = 0;
};

struct EngineDebugAccess : public Proxy<IEngineDebugAccess>
{
    EngineDebugAccess();
};