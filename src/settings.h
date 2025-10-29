#pragma once

#include <iostream>
#include <fstream>
#include "json.hpp"


using json = nlohmann::json;

enum WindowMode {
	BORDERLESS = 0,
	INVISIBLE = 1,
	FULLSCREEN = 2
};

class EngineSettings {
public:
	std::string windowName = "Lengine";
	uint16_t  windowWidth = 1280;
	uint16_t  windowHeight = 720 ;
	WindowMode windowMode = WindowMode::BORDERLESS;

	float cameraPosX = 0;
	float cameraPosY = 0;
	float cameraPosZ = 0;
	float cameraFov = 45;


	const bool loadSettings(const std::string filePath);
};

