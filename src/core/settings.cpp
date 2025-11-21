#include "settings.h"

std::filesystem::path exePath = std::filesystem::current_path();
std::string configPath = (exePath / "../config.json").string();


const bool EngineSettings::loadSettings() {
    std::ifstream file(configPath);
    if (file.fail()) {
        perror(configPath.c_str());
        Lengine::fatalError("Error to open " + configPath);
    }

    json j;
    file >> j;
    
    windowName = j.value("windowName", windowName);
    windowWidth = j.value("windowWidth", windowWidth);
    windowHeight = j.value("windowHeight", windowHeight);
    
    std::string modeStr = j.value("windowMode", "borderless");

    if (modeStr == "borderless")
        windowMode = BORDERLESS;
    else if (modeStr == "invisible")
        windowMode = INVISIBLE;
    else if (modeStr == "fullscreen")
        windowMode = FULLSCREEN;
    else {
        std::cerr << "Unknown windowMode: " << modeStr
            << ", defaulting to borderless.\n";
        windowMode = BORDERLESS;
    }

    cameraPosX = j.value("cameraPosX", cameraPosX);
    cameraPosY = j.value("cameraPosX", cameraPosY);
    cameraPosZ = j.value("cameraPosX", cameraPosZ);
    cameraFov = j.value("cameraFov", cameraFov);

    gameFolderPath = j.value("gameFolderPath", gameFolderPath);

    std::cout << "Loaded settings from " << configPath << "\n";
    return true;
}