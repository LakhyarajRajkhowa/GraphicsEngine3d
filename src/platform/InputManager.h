#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>


namespace Lengine {
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);
		void updateMouseCoords();
		bool isKeyPressed(unsigned int keyID);
		bool isKeyDown(unsigned int keyID);


		glm::vec2 getMouseCoords() const { return _mouseCoords; }


	private:
		bool wasKeyDown(unsigned int keyID);
		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previousKeyMap;

		glm::vec2 _mouseCoords;

	};
}


