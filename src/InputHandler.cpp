#include "InputHandler.h"


namespace Lengine {
    void InputHandler::handleMouseResponse() {
        glm::vec2 mouseCoords = inputManager.getMouseCoords();  // Use InputManager

        int mouseX, mouseY;
        SDL_GetRelativeMouseState(&mouseX, &mouseY);
        camera.processMouse((float)mouseX, (float)mouseY);
        SDL_SetRelativeMouseMode(SDL_TRUE);


    }

    void InputHandler::handleKeyboardResponse() {

        if (inputManager.isKeyPressed(SDLK_ESCAPE))
        {
            isRunning = false;
        }

    }


    void InputHandler::handleInputs() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            inputManager.update();
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                inputManager.pressKey(event.key.keysym.sym);

                break;
            case SDL_KEYUP:
                inputManager.releaseKey(event.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                inputManager.pressKey(event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                inputManager.releaseKey(event.button.button);
                break;
            case SDL_MOUSEMOTION:
                inputManager.setMouseCoords(event.motion.x, event.motion.y);
                break;

            }

        }

        handleMouseResponse();
        handleKeyboardResponse();
        camera.update(0.1f);
    }

}
