#include "engine/level/level.h"

#include "engine/object/baseobject.h"
#include "engine/entity/entity.h"

#include "thirdparty/json.h"

void Level::SetupLevel()
{
    ensure(IsValid());
}

void Level::TeardownLevel()
{
    _isValid = false;
    _name.clear();
    _path.clear();
    _entities.clear();
}

void Level::Serialize(nlohmann::json& data) const
{
    std::vector<nlohmann::json> entitiesData;
    entitiesData.resize(_entities.size());
    for (size_t i = 0; i < _entities.size(); ++i)
    {
        ensure(_entities[i] != nullptr);
        _entities[i]->Serialize(entitiesData[i]);
    }

    data["entities"] = entitiesData;
}

bool Level::Deserialize(const char* fileName_, const nlohmann::json& data_)
{
    // Invalidate on start and only validate if everything was loaded correctly.
    _isValid = false;

    _name = fileName_;

    // Load entities
    for (const auto& entityData : data_["entities"])
    {
        const std::string classStr = entityData["Class"].get<std::string>();
        const Class* classObject = ObjectRepository::FindClass(classStr.c_str());
        ensure(classObject != nullptr);

        // ptodo - ensure types are correct.
        EntityBase* entity = static_cast<EntityBase*>(classObject->InstantiateObject());
        ensure(entity != nullptr);

        entity->Deserialize(fileName_, entityData);

        // ptodo setup entity
        
        entity->EntityCreated();
        
        _entities.emplace_back(entity);

        _onEntityAdded.Broadcast(*entity);
    }
    
    _isValid = true;
    
    return true;
}

void Level::DrawEditor()
{
    IHasEditor::DrawEditor();

    // ptodo
}
