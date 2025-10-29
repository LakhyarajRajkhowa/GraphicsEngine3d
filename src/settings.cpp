#include "settings.h"
#include "Errors.h"

const bool EngineSettings::loadSettings(const std::string path) {
    std::ifstream file(path);
    if (file.fail()) {
        perror(path.c_str());
        Lengine::fatalError("Error to open " + path);
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

    std::cout << "Loaded settings from " << path << "\n";
    return true;
}