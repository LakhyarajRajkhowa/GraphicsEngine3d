#include "InputManager.h"

namespace Lengine {


	InputManager::InputManager() : _mouseCoords(0, 0) {}
	InputManager::~InputManager() {}

	void InputManager::update() {
		for (auto& it : _keyMap) {
			_previousKeyMap[it.first] = it.second;
		}
	}


	void InputManager::pressKey(unsigned int keyID) {
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}

	void InputManager::updateMouseCoords() {
		int mouseX, mouseY;
		SDL_GetRelativeMouseState(&mouseX, &mouseY);
		_mouseCoords.x = mouseX;
		_mouseCoords.y = mouseY;
	}


	bool InputManager::isKeyDown(unsigned int keyID) {
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end()){
			return it->second;
		}
		else {
			return false;
		}
	}

	bool InputManager::isKeyPressed(unsigned int keyID) {
		if (isKeyDown(keyID) && !wasKeyDown(keyID)) {
			return true;
		}
		return false;
	}

	bool InputManager::wasKeyDown(unsigned int keyID) {
		auto it = _previousKeyMap.find(keyID);
		if (it != _previousKeyMap.end()) {
			return it->second;
		}
		else {
			return false;
		}
	}
}

