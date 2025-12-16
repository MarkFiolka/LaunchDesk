#pragma once
#include <string>

enum class ResourcePaths {
    CONFIG,
    APP_ICON,
    ADD_BUTTON_ICON,
    REMOVE_BUTTON_ICON,
};

std::string getFilePath(ResourcePaths path);
