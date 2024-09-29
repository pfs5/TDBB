#include "editormodule_leveleditor.h"

#include "engine/editorengineaccess.h"
#include "engine/engineaccess.h"
#include "engine/entity/entity.h"
#include "errorhandling/assert.h"
#include "imgui/imguihelpers.h"
#include "util/color.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

// EntityManipulator
void EditorModule_LevelEditor::EntityManipulator::Draw(sf::RenderTarget& renderTarget_)
{
    ensure (EntityRef != nullptr);

    const sf::Vector2f position = EntityRef->GetPosition();

    if (IsSelected || IsHovered)
    {
        // Bounds
        sf::RectangleShape bounds { EntityRef->GetBoundsSize() };
        bounds.setOrigin(bounds.getSize() * 0.5f);
        bounds.setFillColor(sf::Color::Transparent);
        bounds.setOutlineColor(IsSelected ? sf::Color::White : Color::Gray);
        bounds.setOutlineThickness(1.f);
        bounds.setPosition(position);
        renderTarget_.draw(bounds);
    }

    if (IsSelected)
    {
        // Free-move
        sf::RectangleShape freeShape { sf::Vector2f{20.f, 20.f} };
        freeShape.setOrigin(sf::Vector2f{ 0.f, 20.f });
        freeShape.setFillColor(sf::Color::Transparent);
        freeShape.setOutlineColor(FreeMoveHovered ? Color::White : Color::Gray);
        freeShape.setOutlineThickness(1.f);
        freeShape.setPosition(position);
        renderTarget_.draw(freeShape);

        // Up
        sf::RectangleShape up{ sf::Vector2f { 2.f, 50.f } };
        up.setOrigin(sf::Vector2f{ 1.f, 50.f});
        up.setFillColor(MoveUpHovered ? Color::Green : Color::DarkGreen);
        up.setPosition(position);
        renderTarget_.draw(up);

        // Right
        sf::RectangleShape right{ sf::Vector2f { 50.f, 2.f } };
        right.setOrigin(sf::Vector2f{ 0.f, 1.f});
        right.setFillColor(MoveRightHovered ? Color::Red : Color::DarkRed);
        right.setPosition(position);
        renderTarget_.draw(right);
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
    ManipulationPositionOffset = mousePosition_ - EntityRef->GetPosition();
}

void EditorModule_LevelEditor::EntityManipulator::StopManipulation()
{
    CurrentManipulation = EManipulationType::None;
}

void EditorModule_LevelEditor::EntityManipulator::UpdateManipulation(sf::Vector2f mousePosition_)
{
    mousePosition_ -= ManipulationPositionOffset;
    
    switch (CurrentManipulation)
    {
        case EManipulationType::MoveRight:
        {
            const sf::Vector2f currentPosition = EntityRef->GetPosition();
            EntityRef->SetPosition(sf::Vector2f{ mousePosition_.x, currentPosition.y });
            break;
        }
        case EManipulationType::MoveUp:
        {
            const sf::Vector2f currentPosition = EntityRef->GetPosition();
            EntityRef->SetPosition(sf::Vector2f{ currentPosition.x, mousePosition_.y });
            break;
        }
        case EManipulationType::FreeMove:
        {
            EntityRef->SetPosition(mousePosition_);
            break;
        }
    }
}

Bounds EditorModule_LevelEditor::EntityManipulator::GetBounds() const
{
    return Bounds::FromCenterSize(EntityRef->GetPosition(), EntityRef->GetBoundsSize());
}

Bounds EditorModule_LevelEditor::EntityManipulator::GetMoveRightBounds() const
{
    const sf::Vector2f entityPosition = EntityRef->GetPosition();
    return Bounds{ entityPosition - sf::Vector2f{ 0.f, 3.f}, entityPosition + sf::Vector2f{ 50.f, 3.f }};
}

Bounds EditorModule_LevelEditor::EntityManipulator::GetMoveUpBounds() const
{
    const sf::Vector2f entityPosition = EntityRef->GetPosition();
    return Bounds{ entityPosition - sf::Vector2f{ 3.f, 50.f}, entityPosition + sf::Vector2f{ 3.f, 0.f }};
}

Bounds EditorModule_LevelEditor::EntityManipulator::GetFreeMoveBounds() const
{
    const sf::Vector2f entityPosition = EntityRef->GetPosition();
    return Bounds{ entityPosition - sf::Vector2f{ 0.f, 20.f}, entityPosition + sf::Vector2f{ 20.f, 20.f }};
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
            manipulator.MoveRightHovered = manipulator.GetMoveRightBounds().IsPointInside(mousePositionF);
            manipulator.MoveUpHovered = manipulator.GetMoveUpBounds().IsPointInside(mousePositionF);
            manipulator.FreeMoveHovered = !manipulator.MoveRightHovered && !manipulator.MoveUpHovered &&
                manipulator.GetFreeMoveBounds().IsPointInside(mousePositionF);

            if (selectPressed)
            {
                if (manipulator.MoveRightHovered) { manipulator.StartManipulation(EManipulationType::MoveRight, mousePositionF); }
                if (manipulator.MoveUpHovered) { manipulator.StartManipulation(EManipulationType::MoveUp, mousePositionF); }
                if (manipulator.FreeMoveHovered) { manipulator.StartManipulation(EManipulationType::FreeMove, mousePositionF); }
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
    _manipulators.emplace_back(entity_);
}

void EditorModule_LevelEditor::OnCurrentInspectableChanged(IInspectable* newInspectable_)
{
    for (EntityManipulator& manipulator : _manipulators)
    {
        manipulator.IsSelected = manipulator.EntityRef == newInspectable_;
    }
}
