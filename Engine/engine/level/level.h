#pragma once
#include "editor/haseditor.h"
#include "serialization/serializable.h"
#include "thirdparty/json_fwd.h"

class EntityBase;

class Level : public ISerializable, public IHasEditor
{
    friend class EngineModule_Level;
    
public:
    // ptodo - level will not be virtual anyone but loaded from a file.
    
    // Use this function to instantiate entities.
    virtual void SetupLevel() {}
    virtual void StartLevel() {}
    virtual void StopLevel() {}

    bool IsValid() const { return _isValid; }
    const std::string& GetName() const { return _name; }
    const std::string& GetPath() const { return _path; }

    // ISerializable
    virtual void Serialize(nlohmann::json& data_) const override;
    virtual bool Deserialize(const char* fileName_, const nlohmann::json& data_) override;

    // IHasEditor
    virtual void DrawEditor() override;
    
private:
    bool _isValid = true;

    std::string _name;
    std::string _path;
    std::vector<EntityBase*> _entities; // ptodo - handles instead of raw pointers
};
