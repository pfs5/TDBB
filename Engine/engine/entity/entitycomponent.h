#pragma once

#include <SFML/OpenGL.hpp>

#include "entitycomponentaccess.h"
#include "engine/object/baseobject.h"
#include "util/typeid.h"

namespace sf
{
    class RenderTarget;
}

class Class;

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

    // BaseObject
    virtual const Class* GetClass() const override { return _class; }

    // ISerializable
    virtual void Serialize(nlohmann::json& data_) const override;
    virtual bool Deserialize(const char* fileName_, const nlohmann::json& data_) override;
    
    const std::string& GetComponentName() const { return _name; }
    const HashType GetComponentNameHash() const { return _nameHash; }
    
protected:
    EntityComponentBase() = default;
    EntityBase& GetOwner() const { return *_owner; }
    void SetupClass(const char* typeName_);
    
private:
    // ptodo - never null after setup.
    EntityBase* _owner = nullptr;
    const Class* _class = nullptr;

    std::string _name;
    HashType _nameHash;

private:
    void SetName(const char* value_);
    
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