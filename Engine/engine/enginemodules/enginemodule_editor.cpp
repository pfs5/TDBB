#include "enginemodule_editor.h"

#include "imgui.h"

void EngineModule_Editor::Init()
{
    Super::Init();

    EngineSettingsAccess::Open(*this);
}

void EngineModule_Editor::Shutdown()
{
    EngineSettingsAccess::Close(*this);

    Super::Shutdown();
}

void EngineModule_Editor::DrawEditor()
{
    Super::DrawEditor();

    ImGui::SeparatorText("Render settings:");

    ImGui::Text("Anti-aliasing: ");
    ImGui::SameLine();
    ImGui::InputInt("##Antialiasing: ", &_renderSettings.AntialiasingLevel);    
}
