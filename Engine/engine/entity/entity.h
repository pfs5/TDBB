#pragma once

#include "errorhandling/assert.h"
#include "math/vector.h"
#include "util/typeid.h"

#include <SFML/System/Vector2.hpp>

#include "entitycomponentaccess.h"
#include "engine/inspectable.h"
#include "util/stringutil.h"

class EntityComponentBase;

namespace sf
{
    class RenderTarget;
}

class EntityBase : public IInspectable
{
public:
    EntityBase() = default;
    virtual ~EntityBase() = default;
    
    virtual std::string GetTypeName() const = 0;
    
    // IInspectable
    virtual const char* GetInspectableName() const override { return _inspectorName.c_str(); }
    virtual void DrawInspectable() override;

    void EntityCreated();
    void Update(float deltaSeconds_);
    void Draw(sf::RenderTarget& renderTarget_);

    const std::vector<EntityComponentBase*>& GetComponents() const { return _components; }
    
    // ptodo - move to private engine access
    void SetEntityName(const char* name_) { _name = name_; }

    const std::string& GetEntityName() const { return _name; }

    void SetPosition(sf::Vector2f value_) { _position = value_; }
    sf::Vector2f GetPosition() const { return _position; }

protected:
    // Entities should create components in their constructors.
    // ptodo - runtime component creation is not yet supported.
    template <typename ComponentType>
    ComponentType* ConstructComponent(const char* name_)
    {
        static_assert(std::is_base_of_v<EntityComponentBase, ComponentType>);
        
        // ptodo - handle this with smart pointers and ref counting somehow
        ComponentType* component = IEntityComponentAccess::CreateComponent<ComponentType>();
        ensure(component != nullptr);
        
        IEntityComponentAccess& componentAccess = static_cast<IEntityComponentAccess&>(*component);
        componentAccess.SetupComponent(*this, name_);

        _components.emplace_back(component);
        
        return component;
    }

    void UpdateInspectorName() { _inspectorName = StringFormat("%s (%s)", _name.c_str(), GetTypeName().c_str()); }
    
protected:
    std::string _inspectorName; // cached to avoid constant recalculation.
    
private:
    std::string _name;
    
    sf::Vector2f _position = ZERO_VECTOR_F;
    std::vector<EntityComponentBase*> _components;
};

template <typename CRTP>
class Entity : public EntityBase
{
public:
    Entity()
    {
        _inspectorName = StringFormat("%s (%s)", GetEntityName().c_str(), GetTypeName().c_str());
    }
    
    // EntityBase
    virtual std::string GetTypeName() const final { return TypeId<CRTP>::GetName(); }
    
};

// Convenience empty entity.
class BasicEntity : public Entity<BasicEntity>
{
    
};