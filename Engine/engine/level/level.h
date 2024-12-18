﻿#pragma once
#include "editor/haseditor.h"
#include "events/delegate.h"
#include "serialization/serializable.h"
#include "thirdparty/json_fwd.h"
#include "util/namedbool.h"

class EntityBase;

class Level : public ISerializable, public IHasEditor
{
    friend class EngineModule_Level;

    using OnEntityAddedDelegate = Delegate<EntityBase& /*entity_*/>;
    
public:
    // ptodo - level will not be virtual anyone but loaded from a file.
    
    // Use this function to instantiate entities.
    virtual void SetupLevel();
    virtual void TeardownLevel();
    virtual void StartLevel() {}
    virtual void StopLevel() {}

    bool IsValid() const { return _isValid; }
    const std::string& GetName() const { return _name; }
    const std::string& GetPath() const { return _path; }

    OnEntityAddedDelegate& OnEntityAdded() { return _onEntityAdded; }
    
    // ISerializable
    virtual void Serialize(nlohmann::json& data_) const override;
    virtual bool Deserialize(const char* fileName_, const nlohmann::json& data_) override;

    // IHasEditor
    virtual void DrawEditor() override;

#ifdef _EDITOR
    void MarkDirty() { _isDirty = true; }

    using PropagateToEntities = NamedBool<class PropagateToEntitiesTag>;
    void ClearDirty(PropagateToEntities propagateToEntities_ = PropagateToEntities{ false });
    
    bool IsDirty() const { return _isDirty; }
#endif //_EDITOR
    
private:
    std::string _name;
    std::string _path;
    std::vector<EntityBase*> _entities; // ptodo - handles instead of raw pointers

    OnEntityAddedDelegate _onEntityAdded;
    
    bool _isValid = true;
    
#ifdef _EDITOR
    bool _isDirty = false;
#endif // _EDITOR

private:
#ifdef _EDITOR
    void OnEntityDirtyChanged(EntityBase& entity_);
#endif // _EDITOR

};
