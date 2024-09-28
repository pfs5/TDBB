#include "entitycomponent.h"

void EntityComponentBase::Serialize(nlohmann::json& data_) const
{
    BaseObject::Serialize(data_);

    data_["class"] = _class->GetName();
    data_["name"] = _name;
}

bool EntityComponentBase::Deserialize(const char* fileName_, const nlohmann::json& data_)
{
    // ptodo

    return true;
}

void EntityComponentBase::SetupClass(const char* typeName_)
{
    _class = ObjectRepository::FindClass(EObjectCategory::EntityComponent, typeName_);
    ensureMsg(_class != nullptr, "Class not found for component. Are you missing a IMPLEMENT_ENTITY_COMPONENT?");
}

void EntityComponentBase::SetName(const char* value_)
{
    _name = value_;
    _nameHash = HashString(value_);
}
