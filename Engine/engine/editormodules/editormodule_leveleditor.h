#pragma once

#include "engine/editormodule.h"
#include "util/enumutil.h"

#include "editor/manipulators/entitymanipulator.h"

// ptodo - editor stuff should be separated from the engine at some point but now is not that point.

class IInspectable;
class EntityBase;

class EditorModule_LevelEditor : public EditorModule<EditorModule_LevelEditor>
{
    using Super = EditorModule<EditorModule_LevelEditor>;

public:
    MAKE_ENUM(EManipulationMode, uint8_t,
        None,

        Position,
        Rotation
    );
    
public:
    using Super::Super;

    virtual void Startup() override;
    virtual void Shutdown() override;

    virtual void Update(float deltaSeconds_) final;
    virtual void Draw(sf::RenderTarget& renderTarget_) final;
    virtual void DrawEditor() final;

    EManipulationMode GetCurrentManipulationMode() const { return _currentManipulationMode; }
    void SetCurrentManipulationMode(EManipulationMode value_) { _currentManipulationMode = value_; }

private:
    std::vector<EntityManipulator> _manipulators;

    EManipulationMode _currentManipulationMode = EManipulationMode::Position;
    
private:
    void OnCurrentLevelChanged();
    void OnEntityAddedToCurrentLevel(EntityBase& entity_);
    void OnCurrentInspectableChanged(IInspectable* newInspectable_);
};
