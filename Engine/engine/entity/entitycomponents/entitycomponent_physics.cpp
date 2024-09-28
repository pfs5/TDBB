#include "engine/entity/entitycomponents/entitycomponent_physics.h"

#include "engine/entity/entity.h"
#include "serialization/serialization.h"

#include "imgui.h"

IMPLEMENT_ENTITY_COMPONENT(EntityComponent_Physics);

void EntityComponent_Physics::Update(float deltaSeconds_)
{
    EntityComponent<EntityComponent_Physics>::Update(deltaSeconds_);

    GetOwner().SetPosition(GetOwner().GetPosition() + _velocity * deltaSeconds_);

    // ptodo - testing
    sf::Vector2f pos = GetOwner().GetPosition();
    if (pos.x > 600.f || pos.x < 50.f)
    {
        _velocity.x *= -1.f;
    }
    if (pos.y > 600.f || pos.y < 50.f)
    {
        _velocity.y *= -1.f;
    }

    pos.x = std::clamp(pos.x, 50.f, 600.f);
    pos.y = std::clamp(pos.y, 50.f, 600.f);
    GetOwner().SetPosition(pos);
}

void EntityComponent_Physics::DrawInspectable()
{
    EntityComponent<EntityComponent_Physics>::DrawInspectable();

    ImGui::BeginTable("EntityComponent_PhysicsProperties", 2, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 100.f);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    ImGui::TableNextRow();
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Velocity");
    ImGui::TableSetColumnIndex(1);
    float inVelocity [] = { _velocity.x, _velocity.y };
    if(ImGui::InputFloat2("##velocity", inVelocity, "%.2f"))
    {
        _velocity.x = inVelocity[0];
        _velocity.y = inVelocity[1];
    }
    ImGui::EndTable();
}

void EntityComponent_Physics::Serialize(nlohmann::json& data_) const
{
    EntityComponent<EntityComponent_Physics>::Serialize(data_);

    data_["velocity"] = _velocity;
}

bool EntityComponent_Physics::Deserialize(const char* fileName_, const nlohmann::json& data_)
{
    // ptodo

    data_["velocity"].template get_to<sf::Vector2f>(_velocity);
    
    return true;
}
