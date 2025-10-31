#pragma once

#include "Lengine.h"
#include "Timer.h"
#include "Window.h"
#include "Camera3d.h"
#include "InputManager.h"
#include "InputHandler.h"
#include "SceneRenderer.h"
#include "UIRenderer.h"
#include "settings.h"


#include "json.hpp"

namespace Lengine {
	class GraphicsEngine {
	public:
		GraphicsEngine();
		void initSettings();
		void run();
	private:
		EngineSettings settings;
		Lengine::Window window;
		Lengine::Camera3d camera;
		Lengine::InputManager inputManager;
		Lengine::InputHandler inputHandler;
		Lengine::AssetManager assetManager;
		Lengine::Scene scene;
		Lengine::SceneRenderer sceneRenderer;
		Lengine::UIRenderer UI;

		bool isRunning = true;
		void initSystems();
	};
}