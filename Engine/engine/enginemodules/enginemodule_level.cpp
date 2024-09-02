#include "engine/enginemodules/enginemodule_level.h"

#include "engine/inspectable.h"
#include "engine/moduleengineaccess.h"
#include "imgui/imguihelpers.h"
#include "rendering/colors.h"
#include "serialization/serialization.h"
#include "thirdparty/json.h"
#include "util/stringutil.h"
#include "engine/entity/entity.h"

void EngineModule_Level::Init()
{
    Super::Init();

    EngineLevelAccess::Open(*this);

    // ptodo - testing
    LoadLevel("X:\\Coding\\Projects\\Games\\tower-defence-but-better\\TowerDefenseButBetter\\resources\\level_showcase.json");
}

void EngineModule_Level::Shutdown()
{
    EngineLevelAccess::Close(*this);

    Super::Shutdown();
}

void EngineModule_Level::DrawEditor()
{
    Super::DrawEditor();

    IMGUI_SCOPED_INDENT();
    
    ImGui::Text("Current level:");
    ImGui::SameLine();
    ImGui::TextColored(ToImVec4(Colors::DarkYellow), "%s", _currentLevel.GetName().c_str());

    ImGui::NewLine();

    if (_currentLevel.IsValid())
    {
        _currentLevel.DrawEditor();

        const std::string titleEntities = StringFormat("Entities (%d):", _currentLevel._entities.size());
        if (ImGui::TreeNodeEx(titleEntities.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_NoTreePushOnOpen))
        {
            IMGUI_SCOPED_INDENT();

            const IInspectable* const currentInspected = _engineAccess->GetCurrentInspected();
            for (EntityBase* entity : _currentLevel._entities)
            {
                ensure(entity != nullptr);

                IMGUI_SCOPED_ID(entity);

                bool selected = entity == currentInspected;
                const std::string titleEntity { StringFormat("%s (%s)", entity->GetEntityName().c_str(), entity->GetTypeName().c_str()) };

                if(ImGui::Selectable(titleEntity.c_str(), &selected))
                {
                    _engineAccess->SetCurrentInspected(entity);                
                }
            }
        }
    }
    
    ImGui::NewLine();
}

void EngineModule_Level::LoadLevel(const char* path_)
{
    nlohmann::json data;
    std::string filename;
    if (Serialization::LoadJsonFromFile(path_, filename, data) == Serialization::ELoadResult::Fail)
    {
        ensure(false);
        return;
    }

    // ptodo - cleanup current level

    _currentLevel.Deserialize(filename.c_str(), data);
    ensure(_currentLevel.IsValid());
}
