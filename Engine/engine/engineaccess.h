#pragma once
#include "util/proxy.h"

// Use these interfaces and proxies to access engine features.

class IEngineLevelAccess
{
    
};

struct EngineLevelAccess : public Proxy<IEngineLevelAccess>
{
    EngineLevelAccess();
};