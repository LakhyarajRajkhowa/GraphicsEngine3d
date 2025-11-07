#include "InputHandler.h"


namespace Lengine {
    void InputHandler::handleMouseResponse() {
        if (!fixCam) {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            SDL_ShowCursor(SDL_ENABLE);
        }
        else {
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }

        inputManager.updateMouseCoords();
        glm::vec2 mouseCoords = inputManager.getMouseCoords();

        int x, y;
        SDL_GetRelativeMouseState(&x, &y);
        glm::vec2 relativeMouseCoords = { x,y };
        moveEntity(mouseCoords);
        camera.update(0.1f, relativeMouseCoords, fixCam);

    }

    void InputHandler::handleKeyboardResponse() {

        if (ImGui::GetIO().WantCaptureKeyboard)
            return;
        for (SDL_Keycode key : { SDLK_ESCAPE, SDLK_c}) {
            if (inputManager.isKeyPressed(key)) {
                switch (key) {
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                case SDLK_c:
                    fixCam = !fixCam;
                    moveMode = !moveMode;
                    break;
                   
                }
            }
        }

        if (inputManager.isKeyDown(SDLK_UP)) {
            if(confirmedSelectedEntity != nullptr)
            confirmedSelectedEntity->getTransform().position.y += 0.01f;
        }
        if (inputManager.isKeyDown(SDLK_DOWN)) {
            if (confirmedSelectedEntity != nullptr)
            confirmedSelectedEntity->getTransform().position.y -= 0.01f;
        }
        if (inputManager.isKeyPressed(SDLK_x)) {
            if(confirmedSelectedEntity != nullptr)
                scene.removeEntity(confirmedSelectedEntity->getName())
                ;
        }
    }

    // this shit is a mess!
    void InputHandler::moveEntity(glm::vec2 mouseCoords) {
        int mouseX = mouseCoords.x, mouseY = mouseCoords.y;

        if (!moveMode) {
            return;
        }

        glm::vec3 rayDir = getRayFromMouse(
            mouseX,
            mouseY,
            window.getScreenWidth(),
            window.getScreenHeight(),
            camera.getViewMatrix(),
            camera.getProjectionMatrix()
        );

        glm::vec3 rayOrigin = camera.getCameraPosition();
        const auto& entities = scene.getEntities();

        if (!SDL_GetMouseState(nullptr, nullptr)) {
            selectedEntity = nullptr;
        }
        else {
            if (selectedEntity != nullptr) {
                glm::vec3 currentHit = RayPlaneIntersection(
                    rayOrigin, rayDir,
                    dragPlaneNormal, dragPlaneY
                );

                if (SDL_BUTTON(SDL_BUTTON_LEFT)) {
                    selectedEntity->getTransform().position = currentHit + dragOffset;
                }
                
                
            }
            else {


                float closest = 999999.0f;

                for (auto& e : entities) {

                    for (auto& sm : e->getMesh()->subMeshes) {
                        float radius = sm.getBoundingRadius() * e->getTransform().scale.x;
                        glm::vec3 centre = e->getTransform().position + sm.getLocalCenter() * e->getTransform().scale;

                        if (rayIntersectsSphere(rayOrigin, rayDir, centre, radius)) {
                            float dist = glm::distance(rayOrigin, centre);
                            if (dist < closest) {
                                closest = dist;
                                selectedEntity = e.get();

                                dragPlaneNormal = glm::vec3(0, 1, 0);
                                dragPlaneY = selectedEntity->getTransform().position.y;

                                dragStartPoint = RayPlaneIntersection(
                                    rayOrigin, rayDir,
                                    dragPlaneNormal, dragPlaneY
                                );

                                dragOffset = selectedEntity->getTransform().position - dragStartPoint;

                                // entity selected
                                if (mouseLeftReleased) {
                                    if (selectedEntity != nullptr) {
                                        confirmSelectedEntity = true;
                                        if (confirmedSelectedEntity != nullptr)confirmedSelectedEntity->isSelected = false;
                                        confirmedSelectedEntity = selectedEntity;
                                        confirmedSelectedEntity->isSelected = true;
                                    }
                                    mouseLeftReleased = false;
                                }
                            }
                        }
                        
                    }
                    
                }

        if(mouseLeftReleased && confirmedSelectedEntity != nullptr) {
            confirmedSelectedEntity->isSelected = false;
            confirmSelectedEntity = false;
            confirmedSelectedEntity = nullptr;
            
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
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouseLeftDown = true;
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseLeftDown = false;
                    mouseLeftReleased = true;  
                }
                break;

            }

            if (event.type == SDL_MOUSEWHEEL) {

                if (event.wheel.y > 0) {
                    if (confirmSelectedEntity && confirmedSelectedEntity != nullptr) {
                        confirmedSelectedEntity->getTransform().scale +=
                            (confirmedSelectedEntity->getTransform().scale.x < 0.1f) ?
                            (confirmedSelectedEntity->getTransform().scale.x < 0.01f) ?
                            glm::vec3(0.001f) : glm::vec3(0.01f) : glm::vec3(0.1f);
                    }
                }
                else if (event.wheel.y < 0) {
                    if (confirmSelectedEntity && confirmedSelectedEntity != nullptr) {
                        confirmedSelectedEntity->getTransform().scale -= 
                            (confirmedSelectedEntity->getTransform().scale.x < 0.1f )?
                                (confirmedSelectedEntity->getTransform().scale.x < 0.01f) ?
                                    glm::vec3(0.001f):glm::vec3(0.01f):glm::vec3(0.1f);
                    }
                }
            }
        }



        handleKeyboardResponse();
        handleMouseResponse();
    }


}