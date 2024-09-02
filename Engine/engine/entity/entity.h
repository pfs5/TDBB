#pragma once

#include "engine/inspectable.h"
#include "entitycomponentaccess.h"
#include "errorhandling/assert.h"
#include "math/vector.h"
#include "util/stringutil.h"
#include "util/typeid.h"
#include "engine/object/baseobject.h"

#include <SFML/System/Vector2.hpp>

class EntityComponentBase;

namespace sf
{
    class RenderTarget;
}

#define IMPLEMENT_ENTITY(Type_) \
    EntityDefinition<Type_> g_##EntityDefinition##Type_;    

class EntityBase : public BaseObject, public IInspectable
{
    using Super = BaseObject;
    
public:
    EntityBase() = default;
    virtual ~EntityBase() = default;
    
    virtual std::string GetTypeName() const = 0;
    
    // IInspectable
    virtual const char* GetInspectableName() const override { return _inspectorName.c_str(); }
    virtual void DrawInspectable() override;

    // ISerializable
    virtual void Serialize(const nlohmann::json& data_) const override;
    virtual bool Deserialize(const char* fileName_, const nlohmann::json& data_) override;
    
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

template <typename EntityType>
struct EntityDefinition : public BaseObjectDefinition<EntityType, EObjectCategory::Entity>
{
    // ptodo - ensure entities use this definition and not BaseObjectDefinition directly
    
    EntityDefinition()
    {
        static_assert(std::is_base_of_v<EntityBase, EntityType> && "T must inherit Entity.");
    }
};

// Convenience empty entity.
class BasicEntity : public Entity<BasicEntity>
{
    
};