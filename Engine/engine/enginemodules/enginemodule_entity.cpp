#include "engine/enginemodules/enginemodule_entity.h"

#include "imgui.h"
#include "engine/moduleengineaccess.h"
#include "engine/entity/entity.h"
#include "engine/entity/entitycomponent.h"
#include "imgui/imguihelpers.h"
#include "util/stringutil.h"

void EngineModule_Entity::Init()
{
    Super::Init();

    EngineEntityAccess::Open(*this);
}

void EngineModule_Entity::Shutdown()
{
    EngineEntityAccess::Close(*this);

    Super::Shutdown();
}

void EngineModule_Entity::Update(float deltaSeconds_)
{
    EngineModule<EngineModule_Entity>::Update(deltaSeconds_);

    ensure(_engineAccess != nullptr);
    if (_engineAccess->IsSimulationActive())
    {
        for (EntityBase* entity : _entities)
        {
            entity->Update(deltaSeconds_);
        }
    }
}

void EngineModule_Entity::Draw(sf::RenderTarget& renderTarget_)
{
    EngineModule<EngineModule_Entity>::Draw(renderTarget_);

    for (EntityBase* entity : _entities)
    {
        entity->Draw(renderTarget_);
    }
}

void EngineModule_Entity::DrawEditor()
{
    EngineModule<EngineModule_Entity>::DrawEditor();

    // ptodo - move this to level module
    const std::string titleEntities = StringFormat("Entities (%d):", _entities.size());
    if (ImGui::TreeNodeEx(titleEntities.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_NoTreePushOnOpen))
    {
        IMGUI_SCOPED_INDENT();

        const IInspectable* const currentInspected = _editorAccess->GetCurrentInspected();
        for (EntityBase* entity : _entities)
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

void EngineModule_Entity::SetupEntity(EntityBase& entity_, sf::Vector2f position_)
{
    entity_.SetPosition(position_);
    _entities.emplace_back(&entity_);
}