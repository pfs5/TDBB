#include "editormodule_leveleditor.h"

#include "engine/editorengineaccess.h"
#include "engine/engineaccess.h"
#include "engine/entity/entity.h"
#include "errorhandling/assert.h"
#include "imgui/imguihelpers.h"
#include "util/color.h"
#include "editor/editorstyle.h"
#include "rendering/shapeutils.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "debug/debugdraw.h"
#include "engine/engineutils/log.h"

// ptodo - a lot of switch/cases here; fix this

// EntityManipulator
void EditorModule_LevelEditor::EntityManipulator::Draw(sf::RenderTarget& renderTarget_)
{
    ensure (EntityRef != nullptr);

    const sf::Vector2f position = EntityRef->GetPosition();
    const float rotationDeg = EntityRef->GetRotationDeg();
    const sf::Vector2i mousePosition = EngineInputsAccess::Get().GetMousePosition();
    const sf::Vector2f mousePositionF { (float)mousePosition.x, (float)mousePosition.y };

    switch (CurrentManipulation)
    {
        case EManipulationType::MoveRight:
        {
            // ptodo
            sf::RectangleShape lineRight { sf::Vector2f{ 4000.f, EditorStyle::LineThickness } };
            lineRight.setOrigin(lineRight.getSize() * 0.5f);
            lineRight.setOutlineColor(sf::Color::Transparent);
            lineRight.setFillColor(sf::Color{ 255, 255, 255, 150 });
            lineRight.setPosition(BaseEntityPosition);
            lineRight.setRotation(rotationDeg);
            renderTarget_.draw(lineRight);
                
            break;
        }
        case EManipulationType::MoveUp:
        {
            // ptodo
            sf::RectangleShape lineUp { sf::Vector2f{ EditorStyle::LineThickness, 4000.f } };
            lineUp.setOrigin(lineUp.getSize() * 0.5f);
            lineUp.setOutlineColor(sf::Color::Transparent);
            lineUp.setFillColor(sf::Color{ 255, 255, 255, 150 });
            lineUp.setPosition(BaseEntityPosition);
            lineUp.setRotation(rotationDeg);
            renderTarget_.draw(lineUp);
                
            break;
        }
        case EManipulationType::Rotate:
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
            case EManipulationMode::Position:
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

            case EManipulationMode::Rotation:
            {
                sf::CircleShape shape { 20.f - EditorStyle::LineThickness };
                shape.setOrigin(sf::Vector2f{ shape.getRadius(), shape.getRadius() });
                shape.setOutlineThickness(EditorStyle::LineThickness);
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

void EditorModule_LevelEditor::EntityManipulator::StartManipulation(EManipulationType type_,
    sf::Vector2f mousePosition_)
{
    CurrentManipulation = type_;
    BaseEntityPosition = EntityRef->GetPosition();
    BaseEntityRotationDeg = EntityRef->GetRotationDeg();
    ManipulationPositionOffset = mousePosition_ - BaseEntityPosition;
}

void EditorModule_LevelEditor::EntityManipulator::StopManipulation()
{
    CurrentManipulation = EManipulationType::None;
}

void EditorModule_LevelEditor::EntityManipulator::UpdateManipulation(sf::Vector2f mousePosition_)
{
    mousePosition_ -= ManipulationPositionOffset;

    // ptodo - clamp movement to x or y axis; take into account rotation
    
    switch (CurrentManipulation)
    {
        case EManipulationType::MoveRight:
        {
            const sf::Vector2f movementVector = Math::RotateVector(RIGHT_VECTOR_F, Math::DegreeToRadian(EntityRef->GetRotationDeg()));
            const sf::Vector2f targetPosition = Math::ProjectPointOntoLine(mousePosition_, BaseEntityPosition, movementVector);
            EntityRef->SetPosition(targetPosition);

            break;
        }
        case EManipulationType::MoveUp:
        {
            const sf::Vector2f movementVector = Math::RotateVector(UP_VECTOR_F, Math::DegreeToRadian(EntityRef->GetRotationDeg()));
            const sf::Vector2f targetPosition = Math::ProjectPointOntoLine(mousePosition_, BaseEntityPosition, movementVector);
            EntityRef->SetPosition(targetPosition);
                
            break;
        }
        case EManipulationType::FreeMove:
        {
            EntityRef->SetPosition(mousePosition_);
            break;
        }
    case EManipulationType::Rotate:
            {
                const sf::Vector2f targetVector = mousePosition_ - EntityRef->GetPosition();
                const float angleDeltaDeg = Math::VectorAngleDeg(ManipulationPositionOffset, targetVector);
                EntityRef->SetRotationDeg(BaseEntityRotationDeg + angleDeltaDeg);
                
                break;
            }
    }
}

BoxCircleBounds EditorModule_LevelEditor::EntityManipulator::GetBounds() const
{
    return BoxCircleBounds::MakeBox(EntityRef->GetPosition(), EntityRef->GetBoundsSize(), EntityRef->GetRotationDeg() );
}

BoxCircleBounds EditorModule_LevelEditor::EntityManipulator::GetMoveRightBounds() const
{
    static const sf::Vector2f SIZE { 50.f, 6.f };
    
    const Transform entityTransform { EntityRef->GetEntityTransform() };

    const sf::Vector2f center { SIZE * 0.5f };
    return BoxCircleBounds::MakeBox(entityTransform.TransformPosition(center), SIZE, EntityRef->GetRotationDeg());
}

BoxCircleBounds EditorModule_LevelEditor::EntityManipulator::GetMoveUpBounds() const
{
    static const sf::Vector2f SIZE { 6.f, 50.f };
    
    const Transform entityTransform { EntityRef->GetEntityTransform() };

    const sf::Vector2f center { SIZE.x * 0.5f, -SIZE.y * 0.5f };
    return BoxCircleBounds::MakeBox(entityTransform.TransformPosition(center), SIZE, EntityRef->GetRotationDeg());
}

BoxCircleBounds EditorModule_LevelEditor::EntityManipulator::GetFreeMoveBounds() const
{
    static const sf::Vector2f SIZE { 20.f, 20.f };
    
    const Transform entityTransform { EntityRef->GetEntityTransform() };

    const sf::Vector2f center { SIZE.x * 0.5f, -SIZE.y * 0.5f };
    return BoxCircleBounds::MakeBox(entityTransform.TransformPosition(center), SIZE, EntityRef->GetRotationDeg());
}

BoxCircleBounds EditorModule_LevelEditor::EntityManipulator::GetRotateBounds() const
{
    static const float RADIUS { 20.f };

    return BoxCircleBounds::MakeCircle(EntityRef->GetPosition(), RADIUS);
}

void EditorModule_LevelEditor::Startup()
{
    Super::Startup();

    ensure(EngineLevelAccess::IsValid());
    EngineLevelAccess::Get().CurrentLevelChanged().AddListener(this, &EditorModule_LevelEditor::OnCurrentLevelChanged);
    EngineLevelAccess::Get().EntityAdded().AddListener(this, &EditorModule_LevelEditor::OnEntityAddedToCurrentLevel);
    
    _engineAccess->CurrentInspectableChanged().AddListener(this, &EditorModule_LevelEditor::OnCurrentInspectableChanged);
}

void EditorModule_LevelEditor::Shutdown()
{
    
    
    Super::Shutdown();
}

void EditorModule_LevelEditor::Update(float deltaSeconds_)
{
    Super::Update(deltaSeconds_);

    ensure(EngineInputsAccess::IsValid());

    const sf::Vector2i mousePosition = EngineInputsAccess::Get().GetMousePosition();
    // ptodo - mouse button to param
    static const sf::Mouse::Button BTN_SELECT { sf::Mouse::Button::Left };
    const bool selectPressed = EngineInputsAccess::Get().MouseButtonPressed(BTN_SELECT);
    const bool selectReleased = EngineInputsAccess::Get().MouseButtonReleased(BTN_SELECT);

    // Update hover
    for (EntityManipulator& manipulator : _manipulators)
    {
        const sf::Vector2f mousePositionF { (float)mousePosition.x, (float)mousePosition.y };
        const bool mouseInBounds = manipulator.GetBounds().IsPointInside(mousePositionF);

        manipulator.UpdateManipulation(mousePositionF);
        
        // Manipulation logic
        if (manipulator.IsSelected)
        {
            switch (_currentManipulationMode)
            {
                case EManipulationMode::Position:
                {
                    manipulator.MoveRightHovered = manipulator.GetMoveRightBounds().IsPointInside(mousePositionF);
                    manipulator.MoveUpHovered = manipulator.GetMoveUpBounds().IsPointInside(mousePositionF);
                    manipulator.FreeMoveHovered = !manipulator.MoveRightHovered && !manipulator.MoveUpHovered &&
                        manipulator.GetFreeMoveBounds().IsPointInside(mousePositionF);
                    
                    break;
                }
                case EManipulationMode::Rotation:
                {
                    manipulator.RotateHovered = manipulator.GetRotateBounds().IsPointInside(mousePositionF);
                        
                    break;
                }
            }

            if (selectPressed)
            {
                if (manipulator.MoveRightHovered) { manipulator.StartManipulation(EManipulationType::MoveRight, mousePositionF); }
                if (manipulator.MoveUpHovered) { manipulator.StartManipulation(EManipulationType::MoveUp, mousePositionF); }
                if (manipulator.FreeMoveHovered) { manipulator.StartManipulation(EManipulationType::FreeMove, mousePositionF); }
                if (manipulator.RotateHovered) { manipulator.StartManipulation(EManipulationType::Rotate, mousePositionF); }
            }

            if (manipulator.IsManipulating() && selectReleased)
            {
                manipulator.StopManipulation();
            }
            
            continue;
        }
        
        // Hover - selection logic
        manipulator.IsHovered = false;
        if (mouseInBounds)
        {
            if (selectPressed)
            {
                _engineAccess->SetCurrentInspected(manipulator.EntityRef);
            }
            else
            {
                manipulator.IsHovered = true;
            }
        }
    }
}

void EditorModule_LevelEditor::Draw(sf::RenderTarget& renderTarget_)
{
    Super::Draw(renderTarget_);

    for (EntityManipulator& manipulator : _manipulators)
    {
        manipulator.Draw(renderTarget_);
    }
}

void EditorModule_LevelEditor::DrawEditor()
{
    Super::DrawEditor();

    IMGUI_SCOPED_INDENT();

    const std::string title = StringFormat("Manipulators (%d): ", _manipulators.size());
    if (ImGui::TreeNodeEx(title.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen))
    {
        IMGUI_SCOPED_INDENT();

        for (size_t idx = 0; idx < _manipulators.size(); ++idx)
        {
            ImGui::Text("[%0.2d] %s (%s)",
                idx, _manipulators[idx].EntityRef->GetEntityName().c_str(), ToString(_manipulators[idx].CurrentManipulation));
        }
    }
}

void EditorModule_LevelEditor::OnCurrentLevelChanged()
{
    _manipulators.clear();
}

void EditorModule_LevelEditor::OnEntityAddedToCurrentLevel(EntityBase& entity_)
{
    _manipulators.emplace_back(entity_, *this);
}

void EditorModule_LevelEditor::OnCurrentInspectableChanged(IInspectable* newInspectable_)
{
    for (EntityManipulator& manipulator : _manipulators)
    {
        manipulator.IsSelected = manipulator.EntityRef == newInspectable_;
    }
}
