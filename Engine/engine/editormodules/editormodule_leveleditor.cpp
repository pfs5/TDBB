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

// ptodo - a lot of switch/cases here; fix this

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
                if (manipulator.MoveRightHovered) { manipulator.StartManipulation(EEntityManipulationType::MoveRight, mousePositionF); }
                if (manipulator.MoveUpHovered) { manipulator.StartManipulation(EEntityManipulationType::MoveUp, mousePositionF); }
                if (manipulator.FreeMoveHovered) { manipulator.StartManipulation(EEntityManipulationType::FreeMove, mousePositionF); }
                if (manipulator.RotateHovered) { manipulator.StartManipulation(EEntityManipulationType::Rotate, mousePositionF); }
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
                idx, _manipulators[idx].EntityRef->GetEntityName().c_str(), ::ToString(_manipulators[idx].CurrentManipulation));
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
