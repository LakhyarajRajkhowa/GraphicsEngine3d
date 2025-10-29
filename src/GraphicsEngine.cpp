#include "GraphicsEngine.h"

namespace Lengine {

	GraphicsEngine::GraphicsEngine(): 
		sceneRenderer(camera),
		inputHandler(camera, inputManager, isRunning)
	{
		
	}
	void GraphicsEngine::initSettings() {
		std::filesystem::path exePath = std::filesystem::current_path();
		std::filesystem::path configPath = exePath / "../../config.json";
		configPath = std::filesystem::absolute(configPath);
		settings.loadSettings(configPath.string());
	}
	void GraphicsEngine::run() {
		initSystems();
	}

	void GraphicsEngine::initSystems() {
		InitTimer();
		Lengine::init();
		window.create(
			settings.windowName,
			settings.windowWidth,
			settings.windowHeight,
			settings.windowMode
			);

		camera.init(
			settings.windowWidth,
			settings.windowHeight,
			&inputManager,
			{ settings.cameraPosX, 5, settings.cameraPosZ },
			settings.cameraFov
		);
		
		sceneRenderer.init();
		sceneRenderer.initScene();
		isRunning = true;

		while (isRunning) {
			
			inputHandler.handleInputs();
			sceneRenderer.beginFrame({ 0.0f, 0.0f, 0.0f, 1.0f });
			sceneRenderer.renderScene();
			UI.renderUI(camera);

			sceneRenderer.endFrame();
			window.swapBuffer();
			
		}
			

	}

}
