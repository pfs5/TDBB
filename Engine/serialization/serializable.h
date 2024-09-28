#pragma once

#include "thirdparty/json_fwd.h"

class ISerializable
{
public:
    virtual void Serialize(nlohmann::json& data_) const = 0;
    virtual bool Deserialize(const char* fileName_, const nlohmann::json& data_) = 0;
};