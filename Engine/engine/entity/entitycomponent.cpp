#include "entitycomponent.h"

#include "entity.h"
#include "imgui.h"
#include "editor/editorstyle.h"

void EntityComponentBase::DrawInspectable()
{
    ImGui::SeparatorText("Properties");

    ImGui::BeginTable("EntityProperties", 2, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 100.f);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    ImGui::TableNextRow(0, EditorStyle::PropertyGridRowHeight);
    
    // ptodo - auto properties
    for (PropertyBase* property : _properties)
    {
        ImGui::TableSetColumnIndex(0);
        ImGui::Text(property->GetName());
        ImGui::TableSetColumnIndex(1);
        property->DrawEditor();

        ImGui::TableNextRow(0, EditorStyle::PropertyGridRowHeight);
    }
    
    ImGui::EndTable();
}

void EntityComponentBase::Serialize(nlohmann::json& data_) const
{
    BaseObject::Serialize(data_);

    for (PropertyBase* property : _properties)
    {
        ensure(property != nullptr);
        property->Serialize(data_);
    }
}

bool EntityComponentBase::Deserialize(const char* fileName_, const nlohmann::json& data_)
{
    for (PropertyBase* property : _properties)
    {
        ensure(property != nullptr);
        property->Deserialize(data_);
    }

    return true;
}

void EntityComponentBase::OnPropertyChanged(PropertyBase* property_)
{
    IPropertyContainer::OnPropertyChanged(property_);

#ifdef _EDITOR
    MarkDirty();
#endif //_EDITOR
}

#ifdef _EDITOR
void EntityComponentBase::MarkDirty()
{
    _owner->MarkDirty();
}
#endif //_EDITOR

void EntityComponentBase::SetupClass(const char* typeName_)
{
    _class = ObjectRepository::FindClass(EObjectCategory::EntityComponent, typeName_);
    ensureMsg(_class != nullptr, "Class not found for component. Are you missing a IMPLEMENT_ENTITY_COMPONENT?");
}

void EntityComponentBase::SetName(const char* value_)
{
    _name = value_;
    _nameHash = HashString(value_);
}
