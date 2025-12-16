#pragma once

#include <string>
#include "../libs/json.hpp"
#include "ResourcePaths.h"

class ConfigManager {
public:
    ConfigManager(ResourcePaths file = ResourcePaths::CONFIG);

    void load();
    void save();

    bool getBool(const std::string& key, bool defaultValue = false);
    void setBool(const std::string& key, bool value);

    bool getGuideViewed(int index);
    void setGuideViewed(int index, bool value);

    bool isFirstStart() const { return firstStart; }

private:
    std::string filePath;
    nlohmann::json config;
    bool firstStart = false;
};
