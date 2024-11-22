#include "engine/entity/entity.h"

#include "entitycomponent.h"
#include "imgui.h"
#include "imgui/imguihelpers.h"
#include "serialization/serialization.h"

#include "editor/editorstyle.h"

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

void EntityBase::Serialize(nlohmann::json& data_) const
{
    Super::Serialize(data_);

    // ptodo - auto serialization
    for (const PropertyBase* const property : _properties)
    {
        ensure(property != nullptr);
        property->Serialize(data_);
    }

    // ptodo - components
    for (const EntityComponentBase* const component : _components)
    {
        ensure(component != nullptr);

        nlohmann::json& componentData = data_["Components"].emplace_back();
        component->Serialize(componentData);
    }
}

bool EntityBase::Deserialize(const char* fileName_, const nlohmann::json& data_)
{
    if (!Super::Deserialize(fileName_, data_))
    {
        return false;
    }

    for (PropertyBase* const property : _properties)
    {
        ensure(property != nullptr);
        property->Deserialize(data_);
    }
    
    // Components
    for (const nlohmann::json& compJson : data_["Components"])
    {
        const std::string compName = compJson["Name"].get<std::string>();
        const std::string compClassStr = compJson["Class"].get<std::string>();
        const Class* const compClass = ObjectRepository::FindClass(EObjectCategory::EntityComponent, compClassStr.c_str());
        ensure(compClass != nullptr);
        
        EntityComponentBase* component = FindComponent(*compClass, compName.c_str());
        ensure(component != nullptr);

        component->Deserialize(fileName_, compJson);
    }

    return true;
}

void EntityBase::OnPropertyChanged(PropertyBase* property_)
{
#ifdef _EDITOR
    MarkDirty();
#endif //_EDITOR
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
}

EntityComponentBase* EntityBase::FindComponent(const char* componentName_) const
{
    const HashType componentNameHash = HashString(componentName_);
    const auto it = std::find_if(_components.begin(), _components.end(),
            [componentNameHash](EntityComponentBase* element_)
            {
                ensure(element_ != nullptr);
                return element_->GetComponentNameHash() == componentNameHash;
            });

    return it != _components.end() ? *it : nullptr;    
}

EntityComponentBase* EntityBase::FindComponent(const Class& class_, const char* componentName_) const
{
    const HashType componentNameHash = HashString(componentName_);
    const auto it = std::find_if(_components.begin(), _components.end(),
            [componentNameHash, &class_](EntityComponentBase* element_)
            {
                ensure(element_ != nullptr);
                return element_->GetComponentNameHash() == componentNameHash && element_->GetClass() == &class_;
            });

    return it != _components.end() ? *it : nullptr;  
}

void EntityBase::SetupClass(const char* typeName_)
{
    _class = ObjectRepository::FindClass(EObjectCategory::Entity, typeName_);
    ensureMsg(_class != nullptr, "Class not found for entity. Are you missing a IMPLEMENT_ENTITY?");
}
