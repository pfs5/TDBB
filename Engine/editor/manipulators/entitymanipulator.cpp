#include "editor/manipulators/entitymanipulator.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "editor/editorstyle.h"
#include "engine/engineaccess.h"
#include "engine/editormodules/editormodule_leveleditor.h"
#include "engine/entity/entity.h"
#include "rendering/shapeutils.h"
#include "util/color.h"

void EntityManipulator::Draw(sf::RenderTarget& renderTarget_)
{
    ensure (EntityRef != nullptr);

    const sf::Vector2f position = EntityRef->GetPosition();
    const float rotationDeg = EntityRef->GetRotationDeg();
    const sf::Vector2i mousePosition = EngineInputsAccess::Get().GetMousePosition();
    const sf::Vector2f mousePositionF { (float)mousePosition.x, (float)mousePosition.y };

    switch (CurrentManipulation)
    {
        case EEntityManipulationType::MoveRight:
        {
            // ptodo
            sf::RectangleShape lineRight { sf::Vector2f{ 4000.f, EditorStyle::ThinLineThickness } };
            lineRight.setOrigin(lineRight.getSize() * 0.5f);
            lineRight.setOutlineColor(sf::Color::Transparent);
            lineRight.setFillColor(sf::Color{ 255, 255, 255, 150 });
            lineRight.setPosition(BaseEntityPosition);
            lineRight.setRotation(rotationDeg);
            renderTarget_.draw(lineRight);
                
            break;
        }
        case EEntityManipulationType::MoveUp:
        {
            // ptodo
            sf::RectangleShape lineUp { sf::Vector2f{ EditorStyle::ThinLineThickness, 4000.f } };
            lineUp.setOrigin(lineUp.getSize() * 0.5f);
            lineUp.setOutlineColor(sf::Color::Transparent);
            lineUp.setFillColor(sf::Color{ 255, 255, 255, 150 });
            lineUp.setPosition(BaseEntityPosition);
            lineUp.setRotation(rotationDeg);
            renderTarget_.draw(lineUp);
                
            break;
        }
        case EEntityManipulationType::Rotate:
        {
            const sf::Vector2f entityToMouse = mousePositionF - position;

            // Base reference line
            sf::RectangleShape refLine = Shapes::MakeLine(Math::NormalizeVector(ManipulationPositionOffset) * Math::VectorSize(entityToMouse), EditorStyle::LineThickness);
            refLine.setPosition(BaseEntityPosition);
            refLine.setFillColor(sf::Color::White);
            renderTarget_.draw(refLine);

            // Current line
            sf::RectangleShape curLine = Shapes::MakeLine(entityToMouse, EditorStyle::LineThickness);
            curLine.setPosition(BaseEntityPosition);
            curLine.setFillColor(Color::Red);
            renderTarget_.draw(curLine);
                
            break;
        }
    }
    
    if (IsSelected || IsHovered)
    {
        // Bounds
        sf::RectangleShape bounds { EntityRef->GetBoundsSize() - sf::Vector2f{ EditorStyle::BoundsThickness, EditorStyle::BoundsThickness } };
        bounds.setOrigin(bounds.getSize() * 0.5f);
        bounds.setFillColor(sf::Color::Transparent);
        bounds.setOutlineColor(IsSelected ? sf::Color::White : Color::Gray);
        bounds.setOutlineThickness(EditorStyle::BoundsThickness);
        bounds.setPosition(position);
        bounds.setRotation(rotationDeg);
        renderTarget_.draw(bounds);
    }

    if (IsSelected)
    {
        // Position mode
        switch(ParentModule->GetCurrentManipulationMode())
        {
            case EditorModule_LevelEditor::EManipulationMode::Position:
            {
                // Free-move
                sf::RectangleShape freeShape { sf::Vector2f{20.f - EditorStyle::BoundsThickness, 20.f - EditorStyle::BoundsThickness} };
                freeShape.setOrigin(sf::Vector2f{ -EditorStyle::BoundsThickness * 0.5f, 20.f - EditorStyle::BoundsThickness * 0.5f });
                freeShape.setFillColor(sf::Color::Transparent);
                freeShape.setOutlineColor(FreeMoveHovered ? Color::White : Color::Gray);
                freeShape.setOutlineThickness(EditorStyle::BoundsThickness);
                freeShape.setPosition(position);
                freeShape.setRotation(rotationDeg);
                renderTarget_.draw(freeShape);

                // Up
                sf::RectangleShape up{ sf::Vector2f { EditorStyle::LineThickness, 50.f } };
                up.setOrigin(sf::Vector2f{ 1.f, 50.f});
                up.setFillColor(MoveUpHovered ? Color::Green : Color::DarkGreen);
                up.setPosition(position);
                up.setRotation(rotationDeg);
                renderTarget_.draw(up);

                // Right
                sf::RectangleShape right{ sf::Vector2f { 50.f, EditorStyle::LineThickness } };
                right.setOrigin(sf::Vector2f{ 0.f, 1.f});
                right.setFillColor(MoveRightHovered ? Color::Red : Color::DarkRed);
                right.setPosition(position);
                right.setRotation(rotationDeg);
                renderTarget_.draw(right);
                    
                break;
            }

            case EditorModule_LevelEditor::EManipulationMode::Rotation:
            {
                sf::CircleShape shape { 30.f - EditorStyle::ThinLineThickness };
                shape.setOrigin(sf::Vector2f{ shape.getRadius(), shape.getRadius() });
                shape.setOutlineThickness(EditorStyle::ThinLineThickness);
                shape.setFillColor(sf::Color::Transparent);
                shape.setOutlineColor(RotateHovered ? Color::Red : Color::DarkRed);
                shape.setPosition(position);
                renderTarget_.draw(shape);
                    
                break;
            }
        }
    }

    if (IsSelected || IsHovered)
    {
        // Center
        sf::CircleShape center { 4.f };
        center.setFillColor(sf::Color::White);
        center.setOutlineColor(sf::Color::Black);
        center.setOutlineThickness(2.f);
        center.setOrigin(sf::Vector2f{ 4.f, 4.f });
        center.setPosition(position);
        renderTarget_.draw(center);   
    }
}

void EntityManipulator::StartManipulation(EEntityManipulationType type_,
    sf::Vector2f mousePosition_)
{
    CurrentManipulation = type_;
    BaseEntityPosition = EntityRef->GetPosition();
    BaseEntityRotationDeg = EntityRef->GetRotationDeg();
    ManipulationPositionOffset = mousePosition_ - BaseEntityPosition;
}

void EntityManipulator::StopManipulation()
{
    CurrentManipulation = EEntityManipulationType::None;
}

void EntityManipulator::UpdateManipulation(sf::Vector2f mousePosition_)
{
    mousePosition_ -= ManipulationPositionOffset;

    // ptodo - clamp movement to x or y axis; take into account rotation
    
    switch (CurrentManipulation)
    {
        case EEntityManipulationType::MoveRight:
        {
            const sf::Vector2f movementVector = Math::RotateVector(RIGHT_VECTOR_F, Math::DegreeToRadian(EntityRef->GetRotationDeg()));
            const sf::Vector2f targetPosition = Math::ProjectPointOntoLine(mousePosition_, BaseEntityPosition, movementVector);
            EntityRef->SetPosition(targetPosition);

            break;
        }
        case EEntityManipulationType::MoveUp:
        {
            const sf::Vector2f movementVector = Math::RotateVector(UP_VECTOR_F, Math::DegreeToRadian(EntityRef->GetRotationDeg()));
            const sf::Vector2f targetPosition = Math::ProjectPointOntoLine(mousePosition_, BaseEntityPosition, movementVector);
            EntityRef->SetPosition(targetPosition);
                
            break;
        }
        case EEntityManipulationType::FreeMove:
        {
            EntityRef->SetPosition(mousePosition_);
            break;
        }
        case EEntityManipulationType::Rotate:
        {
            const sf::Vector2f targetVector = mousePosition_ - EntityRef->GetPosition() + ManipulationPositionOffset;
            const float angleDeltaDeg = Math::VectorAngleDeg(ManipulationPositionOffset, targetVector);
            EntityRef->SetRotationDeg(BaseEntityRotationDeg + angleDeltaDeg);

            break;
        }
    }
}

BoxCircleBounds EntityManipulator::GetBounds() const
{
    return BoxCircleBounds::MakeBox(EntityRef->GetPosition(), EntityRef->GetBoundsSize(), EntityRef->GetRotationDeg() );
}

BoxCircleBounds EntityManipulator::GetMoveRightBounds() const
{
    static const sf::Vector2f SIZE { 50.f, 6.f };
    
    const Transform entityTransform { EntityRef->GetEntityTransform() };

    const sf::Vector2f center { SIZE * 0.5f };
    return BoxCircleBounds::MakeBox(entityTransform.TransformPosition(center), SIZE, EntityRef->GetRotationDeg());
}

BoxCircleBounds EntityManipulator::GetMoveUpBounds() const
{
    static const sf::Vector2f SIZE { 6.f, 50.f };
    
    const Transform entityTransform { EntityRef->GetEntityTransform() };

    const sf::Vector2f center { SIZE.x * 0.5f, -SIZE.y * 0.5f };
    return BoxCircleBounds::MakeBox(entityTransform.TransformPosition(center), SIZE, EntityRef->GetRotationDeg());
}

BoxCircleBounds EntityManipulator::GetFreeMoveBounds() const
{
    static const sf::Vector2f SIZE { 20.f, 20.f };
    
    const Transform entityTransform { EntityRef->GetEntityTransform() };

    const sf::Vector2f center { SIZE.x * 0.5f, -SIZE.y * 0.5f };
    return BoxCircleBounds::MakeBox(entityTransform.TransformPosition(center), SIZE, EntityRef->GetRotationDeg());
}

BoxCircleBounds EntityManipulator::GetRotateBounds() const
{
    static const float RADIUS { 30.f };

    return BoxCircleBounds::MakeCircle(EntityRef->GetPosition(), RADIUS);
}
