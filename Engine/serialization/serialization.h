#pragma once

#include "thirdparty/json_fwd.h"
#include <SFML/System/Vector2.hpp>

namespace Serialization
{
    enum class ELoadResult : uint8_t { Success, Fail };
    ELoadResult LoadJsonFromFile(const char* path_, std::string& outFileName_, nlohmann::json& outJson_);

    enum class ESaveResult : uint8_t { Success, Fail };
    ESaveResult SaveJsonToFile(const char* path_, const nlohmann::json& json_);

}

// Custom json serialization for common types.
namespace sf
{
    void to_json(nlohmann::json& jsonObj_, const sf::Vector2f& vector_);
    void from_json(const nlohmann::json& jsonObj_, sf::Vector2f& vector_);
}
