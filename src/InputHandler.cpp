#include "InputHandler.h"


namespace Lengine {
    void InputHandler::handleMouseResponse() {
        inputManager.updateMouseCoords();
        glm::vec2 mouseCoords = inputManager.getMouseCoords(); 

        camera.update(0.1f, mouseCoords, fixCam);
        
    }

    void InputHandler::handleKeyboardResponse() {

        if (ImGui::GetIO().WantCaptureKeyboard)
            return;
        for (SDL_Keycode key : { SDLK_ESCAPE, SDLK_c }) {
            if (inputManager.isKeyPressed(key)) {
                switch (key) {
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                case SDLK_c:
                    fixCam = !fixCam;
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    break;
                }
            }
        }
    }



    void InputHandler::handleInputs() {
        inputManager.update();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
           
            switch (event.type) {
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
            }
        }

        

        handleKeyboardResponse();
        handleMouseResponse();
    }


}
