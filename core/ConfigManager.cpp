#include "ConfigManager.h"
#include <fstream>
#include "ResourcePaths.h"

using json = nlohmann::json;

ConfigManager::ConfigManager(ResourcePaths file) {
    filePath = getFilePath(file);
}

void ConfigManager::load() {
    std::ifstream f(filePath);

    if (!f.is_open()) {
        firstStart = true;

        config = {
            {"firstStart", false},
            {"showWelcome", true}
        };

        save();
        return;
    }

    f >> config;
}

void ConfigManager::save() {
    std::ofstream f(filePath);
    f << config.dump(4);
}

bool ConfigManager::getBool(const std::string &key, bool defaultValue) {
    return config.value(key, defaultValue);
}

void ConfigManager::setBool(const std::string &key, bool value) {
    config[key] = value;
}

bool ConfigManager::getGuideViewed(int index) {
    std::string key = "guideViewed_" + std::to_string(index);
    return getBool(key, false);
}

void ConfigManager::setGuideViewed(int index, bool value) {
    std::string key = "guideViewed_" + std::to_string(index);
    setBool(key, value);
}
