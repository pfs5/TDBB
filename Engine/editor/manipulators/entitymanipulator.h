#pragma once

#include "math/box.h"
#include "math/vector.h"
#include "util/enumutil.h"

#include <SFML/System/Vector2.hpp>

#include <cstdint>

namespace sf
{
    class RenderTarget;
}

class EntityBase;
class EditorModule_LevelEditor;

MAKE_ENUM(EEntityManipulationType, uint8_t,
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
        
    EEntityManipulationType CurrentManipulation = EEntityManipulationType::None;
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
    void StartManipulation(EEntityManipulationType type_, sf::Vector2f mousePosition_);
    void StopManipulation();
    bool IsManipulating() const { return CurrentManipulation != EEntityManipulationType::None; }
    void UpdateManipulation(sf::Vector2f mousePosition_);
        
    BoxCircleBounds GetBounds() const;
    BoxCircleBounds GetMoveRightBounds() const;
    BoxCircleBounds GetMoveUpBounds() const;
    BoxCircleBounds GetFreeMoveBounds() const;
    BoxCircleBounds GetRotateBounds() const;
        
};
