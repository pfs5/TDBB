#pragma once

#include "engine/editormodule.h"
#include "math/box.h"
#include "math/vector.h"
#include "util/enumutil.h"

#include <SFML/System/Vector2.hpp>

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
    MAKE_ENUM(EManipulationType, uint8_t,
        None,
        
        MoveRight,
        MoveUp,
        FreeMove,

        Rotate
    );
    
    struct EntityManipulator
    {
        EntityManipulator(EntityBase& entity_, EditorModule_LevelEditor& parentEditor_):
            EntityRef{ &entity_ },
            ParentModule{ &parentEditor_ }
        {
            
        }
        // ptodo - this should be a handle
        EntityBase* EntityRef;

        // ptodo - solve this without having a ref to the parent
        EditorModule_LevelEditor* ParentModule;
        
        EManipulationType CurrentManipulation = EManipulationType::None;
        sf::Vector2f ManipulationPositionOffset = ZERO_VECTOR_F;    // Mouse-to-entity offset on manipulation start.
        sf::Vector2f BaseEntityPosition = ZERO_VECTOR_F;            // Entity position on manipulation start.
        float BaseEntityRotationDeg = 0.f;                             // Entity rotation on manipulation start.
        
        bool IsSelected = false;
        bool IsHovered = false;
        bool MoveRightHovered = false;
        bool MoveUpHovered = false;
        bool FreeMoveHovered = false;
        bool RotateHovered = false;
        
        void Draw(sf::RenderTarget& renderTarget_);
        void StartManipulation(EManipulationType type_, sf::Vector2f mousePosition_);
        void StopManipulation();
        bool IsManipulating() const { return CurrentManipulation != EManipulationType::None; }
        void UpdateManipulation(sf::Vector2f mousePosition_);
        
        BoxCircleBounds GetBounds() const;
        BoxCircleBounds GetMoveRightBounds() const;
        BoxCircleBounds GetMoveUpBounds() const;
        BoxCircleBounds GetFreeMoveBounds() const;
        BoxCircleBounds GetRotateBounds() const;
        
    };

private:
    std::vector<EntityManipulator> _manipulators;

    EManipulationMode _currentManipulationMode = EManipulationMode::Position;
    
private:
    void OnCurrentLevelChanged();
    void OnEntityAddedToCurrentLevel(EntityBase& entity_);
    void OnCurrentInspectableChanged(IInspectable* newInspectable_);
};
