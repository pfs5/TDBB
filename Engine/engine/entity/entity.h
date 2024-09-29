#pragma once

#include "engine/inspectable.h"
#include "entitycomponentaccess.h"
#include "errorhandling/assert.h"
#include "math/vector.h"
#include "util/stringutil.h"
#include "util/typeid.h"
#include "engine/object/baseobject.h"

#include <SFML/System/Vector2.hpp>

class Class;
class EntityComponentBase;

namespace sf
{
    class RenderTarget;
}

#define IMPLEMENT_ENTITY(Type_) \
    EntityDefinition<Type_> g_##EntityDefinition##Type_;    

// Don't implement this class directly.
class EntityBase : public BaseObject, public IInspectable
{
    using Super = BaseObject;
    
public:
    EntityBase() = default;
    virtual ~EntityBase() = default;
    
    virtual std::string GetTypeName() const = 0;

    // BaseObject
    virtual const Class* GetClass() const override { return _class; }
    
    // IInspectable
    virtual const char* GetInspectableName() const override { return _inspectorName.c_str(); }
    virtual void DrawInspectable() override;

    // ISerializable
    virtual void Serialize(nlohmann::json& data_) const override;
    virtual bool Deserialize(const char* fileName_, const nlohmann::json& data_) override;
    
    void EntityCreated();
    virtual void Update(float deltaSeconds_);
    virtual void Draw(sf::RenderTarget& renderTarget_);

    const std::vector<EntityComponentBase*>& GetComponents() const { return _components; }
    EntityComponentBase* FindComponent(const char* componentName_) const;
    EntityComponentBase* FindComponent(const Class& class_, const char* componentName_) const;
    
    // ptodo - move to private engine access
    void SetEntityName(const char* name_) { _name = name_; }

    const std::string& GetEntityName() const { return _name; }

    void SetPosition(sf::Vector2f value_) { _position = value_; }
    sf::Vector2f GetPosition() const { return _position; }

    sf::Vector2f GetBoundsSize() const { return _boundsSize; }

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

protected:
    void SetBoundsSize(float x_, float y_) { _boundsSize.x = x_; _boundsSize.y = y_; }
    void SetupClass(const char* typeName_);
    
private:
    std::string _name;
    const Class* _class = nullptr;
    
    sf::Vector2f _position = ZERO_VECTOR_F;
    sf::Vector2f _boundsSize = ZERO_VECTOR_F;
    
    std::vector<EntityComponentBase*> _components;
};

template <typename CRTP>
class Entity : public EntityBase
{
public:
    Entity()
    {
        _inspectorName = StringFormat("%s (%s)", GetEntityName().c_str(), GetTypeName().c_str());
        SetupClass(Entity<CRTP>::GetTypeName().c_str());
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