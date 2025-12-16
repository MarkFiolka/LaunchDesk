#include "ResourcePaths.h"
#include <string>


std::string getFilePath(ResourcePaths path) {
    switch (path) {
        case ResourcePaths::CONFIG:
            return "config.json";
        case ResourcePaths::APP_ICON:
            return ":/icons/icon.svg";
        case ResourcePaths::ADD_BUTTON_ICON:
            return ":/icons/Add_Button.svg";
        case ResourcePaths::REMOVE_BUTTON_ICON:
            return ":/icons/Remove_Button.svg";
    }

    return "";
}
