#pragma once

#include "Camera3d.h"
#include "InputManager.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <imgui/backends/imgui_impl_sdl2.h>

namespace Lengine {
	class InputHandler {
	public:
		InputHandler(Lengine::Camera3d& cam, Lengine::InputManager& inputmgr, bool& running) : 
			camera(cam), inputManager(inputmgr), isRunning(running)
		{
			fixCam = false;
		}

		void handleInputs();
	private:
		Lengine::Camera3d& camera;
		Lengine::InputManager& inputManager;
		bool& isRunning;
		bool fixCam;


		void handleMouseResponse();
		void handleKeyboardResponse();
	};
}