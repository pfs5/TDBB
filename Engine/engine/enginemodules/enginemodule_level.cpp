#include "pch.h"
#include "engine/enginemodules/enginemodule_level.h"

void EngineModule_Level::Init()
{
    Super::Init();

    EngineLevelAccess::Open(*this);
}

void EngineModule_Level::Shutdown()
{
    EngineLevelAccess::Close(*this);

    Super::Shutdown();
}