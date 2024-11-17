#pragma once

#include "entitycomponentaccess.h"
#include "engine/object/baseobject.h"
#include "util/typeid.h"
#include "engine/entity/property.h"
#include "engine/entity/propertycontainer.h"

class PropertyBase;

namespace sf
{
    class RenderTarget;
}

class Class;

#define IMPLEMENT_ENTITY_COMPONENT(Type_) \
    EntityComponentDefinition<Type_> g_##EntityComponentDefinition##Type_;   

#define COMPONENT_PROPERTY(ComponentType_, Type_, Name_, SerializationName_, DefaultValue_) \
    ComponentProperty<Type_, ComponentType_> (Name_){ *this, _properties, SerializationName_, DefaultValue_ };

#define COMPONENT_PROPERTY_CALLBACK(EntityType_, Type_, Name_, SerializationName_, DefaultValue, OnChangeCallback_) \
    ComponentProperty<Type_, EntityType_> (Name_){ *this, _properties, SerializationName_, DefaultValue, OnChangeCallback_ };

class EntityComponentBase
    : public BaseObject
    , public IEntityComponentAccess
    , public IPropertyContainer
{
    friend class IEntityComponentAccess;
    
public:
    virtual ~EntityComponentBase() = default;
    
    virtual std::string GetTypeName() const = 0;

    virtual void Update(float deltaSeconds_) {}
    virtual void Draw(sf::RenderTarget& renderTarget_) {}

    virtual void DrawInspectable();

    // BaseObject
    virtual const Class* GetClass() const override { return _class; }

    // ISerializable
    virtual void Serialize(nlohmann::json& data_) const override;
    virtual bool Deserialize(const char* fileName_, const nlohmann::json& data_) override;
    
    // IPropertyContainer
    virtual void OnPropertyChanged(PropertyBase* property_) override;
    
    const std::string& GetComponentName() const { return _name; }
    const HashType GetComponentNameHash() const { return _nameHash; }

#ifdef _EDITOR
    void MarkDirty();
#endif _EDITOR

protected:
    std::vector<PropertyBase*> _properties; // must be defined before all properties.
    
protected:
    EntityComponentBase() = default;
    EntityBase& GetOwner() const { return *_owner; }
    void SetupClass(const char* typeName_);
    
private:
    COMPONENT_PROPERTY_CALLBACK(EntityComponentBase, std::string, _name, "Name", "", &EntityComponentBase::OnNameChanged);
    COMPONENT_PROPERTY(EntityComponentBase, const Class*, _class, "Class", nullptr);
    
    // ptodo - never null after setup.
    EntityBase* _owner = nullptr;

    HashType _nameHash;

private:
    void SetName(const char* value_);
    void OnNameChanged() { SetName(_name->c_str()); }
    
    // IEntityComponentAccess
    virtual void SetupComponent(EntityBase& owner_, const char* name_) override
    {
        _owner = &owner_;
        SetName(name_);        
    }
    
};

template <typename CRTP>
class EntityComponent : public EntityComponentBase
{
    using Super = EntityComponentBase;

public:
    EntityComponent()
    {
        SetupClass(EntityComponent<CRTP>::GetTypeName().c_str());
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