﻿#pragma once

#include "entitycomponentaccess.h"
#include "engine/object/baseobject.h"
#include "util/typeid.h"

namespace sf
{
    class RenderTarget;
}

#define IMPLEMENT_ENTITY_COMPONENT(Type_) \
    EntityComponentDefinition<Type_> g_##EntityComponentDefinition##Type_;   

class EntityComponentBase : public BaseObject, public IEntityComponentAccess
{
    friend class IEntityComponentAccess;
    
public:
    virtual ~EntityComponentBase() = default;
    
    virtual std::string GetTypeName() const = 0;

    virtual void Update(float deltaSeconds_) {}
    virtual void Draw(sf::RenderTarget& renderTarget_) {}

    virtual void DrawInspectable() {}
    
    const std::string& GetComponentName() const { return _name; }
    
protected:
    EntityComponentBase() = default;
    EntityBase& GetOwner() const { return *_owner; }
    
private:
    // ptodo - never null after setup.
    EntityBase* _owner = nullptr;
    std::string _name;

private:
    // IEntityComponentAccess
    virtual void SetupComponent(EntityBase& owner_, const char* name_) override
    {
        _owner = &owner_;
        _name = name_;
    }
    
};

template <typename CRTP>
class EntityComponent : public EntityComponentBase
{
    using Super = EntityComponentBase;

public:
    EntityComponent()
    {
        
    }
    
    virtual std::string GetTypeName() const final { return TypeId<CRTP>::GetName(); }

};

template <typename ComponentType>
struct EntityComponentDefinition : public BaseObjectDefinition<ComponentType, EObjectCategory::EntityComponent>
{
    // ptodo - ensure entity components use this definition and not BaseObjectDefinition directly
    
    EntityComponentDefinition()
    {
        static_assert(std::is_base_of_v<EntityComponentBase, ComponentType> && "T must inherit EntityComponent.");
    }
};