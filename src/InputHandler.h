#pragma once

#include "Camera3d.h"
#include "InputManager.h"
#include "Scene.h"
#include "Window.h"
#include "ray.h"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <imgui/backends/imgui_impl_sdl2.h>

namespace Lengine {
	class InputHandler {
	public:
		InputHandler(Camera3d& cam, InputManager& inputmgr, Scene& scn, Window& win, bool& running) :
			camera(cam), inputManager(inputmgr), scene(scn), window(win), isRunning(running)
		{
			fixCam = false;
			moveMode = false;
		}

		void handleInputs();
	private:
		Camera3d& camera;
		InputManager& inputManager;
		Scene& scene;
		Window& window;
		bool& isRunning;
		bool fixCam;
		bool moveMode;
		bool confirmSelectedEntity = false;

		bool mouseLeftDown = false;
		bool mouseLeftReleased = false;

		Entity* confirmedSelectedEntity = nullptr;

		glm::vec3 dragPlaneNormal = glm::vec3(0, 1, 0);  // XZ plane (Y up)
		float dragPlaneHeight;
		glm::vec3 dragStartOffset;

		float dragPlaneY = 0.0f;
		glm::vec3 dragStartPoint;
		glm::vec3 dragOffset;

		Entity* selectedEntity = nullptr;
		int lastMouseX = 0;
		int lastMouseY = 0;

		void handleMouseResponse();
		void handleKeyboardResponse();

		void moveEntity(glm::vec2 mouseCooords);
	};
}