#pragma once

#include "engine/editormodule.h"
#include "math/bounds.h"
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
    using Super::Super;

    virtual void Startup() override;
    virtual void Shutdown() override;

    virtual void Update(float deltaSeconds_) final;
    virtual void Draw(sf::RenderTarget& renderTarget_) final;
    virtual void DrawEditor() final;

private:
    MAKE_ENUM(EManipulationType, uint8_t,
        None,
        
        MoveRight,
        MoveUp,
        FreeMove
    );
    
    struct EntityManipulator
    {
        EntityManipulator(EntityBase& entity_):
            EntityRef{ &entity_ }
        {
            
        }
        // ptodo - this should be a handle
        EntityBase* EntityRef;

        EManipulationType CurrentManipulation = EManipulationType::None;
        sf::Vector2f ManipulationPositionOffset = ZERO_VECTOR_F;
        
        bool IsSelected = false;
        bool IsHovered = false;
        bool MoveRightHovered = false;
        bool MoveUpHovered = false;
        bool FreeMoveHovered = false;
        
        void Draw(sf::RenderTarget& renderTarget_);
        void StartManipulation(EManipulationType type_, sf::Vector2f mousePosition_);
        void StopManipulation();
        bool IsManipulating() const { return CurrentManipulation != EManipulationType::None; }
        void UpdateManipulation(sf::Vector2f mousePosition_);
        
        Bounds GetBounds() const;
        Bounds GetMoveRightBounds() const;
        Bounds GetMoveUpBounds() const;
        Bounds GetFreeMoveBounds() const;
    };

private:
    std::vector<EntityManipulator> _manipulators;
    
private:
    void OnCurrentLevelChanged();
    void OnEntityAddedToCurrentLevel(EntityBase& entity_);
    void OnCurrentInspectableChanged(IInspectable* newInspectable_);
};
