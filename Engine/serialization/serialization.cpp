#include "serialization/serialization.h"

#include <fstream>

Serialization::ELoadResult Serialization::LoadJsonFromFile(const char* path_, std::string& outFileName_, nlohmann::json& outJson_)
{
    const std::filesystem::path path { path_ };
    if (is_empty(path))
    {
        return ELoadResult::Fail;
    }
    outFileName_ = path.stem().string();
    
    std::ifstream file{ path_ };
    if (!file.is_open())
    {
        return ELoadResult::Fail;
    }

    outJson_ = nlohmann::json::parse(file);
    return ELoadResult::Success;
}

Serialization::ESaveResult Serialization::SaveJsonToFile(const char* path_, const nlohmann::json& json_)
{
    std::ofstream file { path_ };
    if (!file.is_open())
    {
        return ESaveResult::Fail;
    }

    file << std::setw(4) << json_ << std::endl;
    return ESaveResult::Success;
}

void sf::to_json(nlohmann::json& jsonObj_, const sf::Vector2f& vector_)
{
    jsonObj_ = nlohmann::json{ {"x", vector_.x}, {"y", vector_.y} };
}

void sf::from_json(const nlohmann::json& jsonObj_, sf::Vector2f& vector_)
{
    jsonObj_.at("x").get_to(vector_.x);
    jsonObj_.at("y").get_to(vector_.y);
}
