#include "engine/entity/entity.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "entitycomponent.h"
#include "imgui.h"
#include "imgui/imguihelpers.h"

IMPLEMENT_ENTITY(BasicEntity);

void EntityBase::DrawInspectable()
{
    IInspectable::DrawInspectable();

    ImGui::NewLine();
    ImGui::SeparatorText("Properties");

    ImGui::BeginTable("EntityProperties", 2, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 100.f);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    ImGui::TableNextRow();
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Position");
    ImGui::TableSetColumnIndex(1);
    float inPosition [] = { _position.x, _position.y };
    if(ImGui::InputFloat2("##position", inPosition, "%.2f"))
    {
        SetPosition(sf::Vector2f{ inPosition[0], inPosition[1] });
    }
    ImGui::EndTable();

    ImGui::NewLine();
    ImGui::SeparatorText("Components");

    const std::string componentsTitle = StringFormat("Components (%d):", _components.size());
    if (ImGui::CollapsingHeader(componentsTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        IMGUI_SCOPED_INDENT();

        for (EntityComponentBase* component : _components)
        {
            ensure(component != nullptr);
            
            const std::string compTitle = StringFormat("%s (%s)", component->GetComponentName().c_str(), component->GetTypeName().c_str());
            if (ImGui::CollapsingHeader(compTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                component->DrawInspectable();
            }
        }
    }
}

void EntityBase::EntityCreated()
{
    UpdateInspectorName();
}

void EntityBase::Update(float deltaSeconds_)
{
    for (EntityComponentBase* component : _components)
    {
        component->Update(deltaSeconds_);
    }
}

void EntityBase::Draw(sf::RenderTarget& renderTarget_)
{
    for (EntityComponentBase* component : _components)
    {
        component->Draw(renderTarget_);
    }
    
    // ptodo - testing
    sf::RectangleShape shape { sf::Vector2f{20.f, 20.f} };
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(_position);
    renderTarget_.draw(shape);
}
