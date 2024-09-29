#include "engine/enginemodules/enginemodule_level.h"

#include "engine/inspectable.h"
#include "engine/moduleengineaccess.h"
#include "engine/engineutils/log.h"
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
}

void EngineModule_Level::OnEngineStarted()
{
    Super::OnEngineStarted();

    // ptodo - testing
    LoadLevel("X:\\Coding\\Projects\\Games\\tower-defence-but-better\\TowerDefenseButBetter\\resources\\level_showcase.json");
}

void EngineModule_Level::Shutdown()
{
    EngineLevelAccess::Close(*this);

    Super::Shutdown();
}

void EngineModule_Level::Update(float deltaSeconds_)
{
    Super::Update(deltaSeconds_);

    ensure(_engineAccess != nullptr);
    if (_engineAccess->IsSimulationActive())
    {
        for (EntityBase* entity : _currentLevel._entities)
        {
            ensure(entity != nullptr);
            entity->Update(deltaSeconds_);
        }   
    }
}

void EngineModule_Level::Draw(sf::RenderTarget& renderTarget_)
{
    Super::Draw(renderTarget_);

    for (EntityBase* entity : _currentLevel._entities)
    {
        ensure(entity != nullptr);
        entity->Draw(renderTarget_);
    }
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

            const IInspectable* const currentInspected = _editorAccess->GetCurrentInspected();
            for (EntityBase* entity : _currentLevel._entities)
            {
                ensure(entity != nullptr);

                IMGUI_SCOPED_ID(entity);

                bool selected = entity == currentInspected;
                const std::string titleEntity { StringFormat("%s (%s)", entity->GetEntityName().c_str(), entity->GetTypeName().c_str()) };

                if(ImGui::Selectable(titleEntity.c_str(), &selected))
                {
                    _editorAccess->SetCurrentInspected(entity);                
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

    _currentLevel.TeardownLevel();

    _currentLevelChanged.Broadcast();
    
    // Init callbacks
    _currentLevel.OnEntityAdded().AddListener(this, &EngineModule_Level::OnEntityAddedToCurrentLevel);
    
    _currentLevel.Deserialize(filename.c_str(), data);
    _currentLevel._path = path_;
    ensure(_currentLevel.IsValid());
}

void EngineModule_Level::SaveCurrentLevel() const
{
    ensure(_currentLevel.IsValid());

    nlohmann::json data;
    _currentLevel.Serialize(data);

    Serialization::SaveJsonToFile(_currentLevel.GetPath().c_str(), data);
}

void EngineModule_Level::OnEntityAddedToCurrentLevel(EntityBase& entity_)
{
    _entityAdded.Broadcast(entity_);
}
