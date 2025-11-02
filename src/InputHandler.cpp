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
    }

    void InputHandler::moveEntity(glm::vec2 mouseCoords) {
        int mouseX = mouseCoords.x, mouseY = mouseCoords.y;

        if (moveMode) {

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

            // -------------------------
            // STEP 1: Select object
            // -------------------------
            if (SDL_GetMouseState(nullptr, nullptr)) {
                if (selectedEntity == nullptr) {
                    float closest = 999999.0f;

                    for (auto& e : entities) {
                        if (e->getName() == "grid") continue;

                        float radius = e->getMesh()->getBoundingRadius();

                        if (rayIntersectsSphere(rayOrigin, rayDir, e->getTransform().position + e.get()->mesh->getLocalCenter(), radius)) {
                            float dist = glm::distance(rayOrigin, e->getTransform().position + e.get()->mesh->getLocalCenter());
                            if (dist < closest) {
                                closest = dist;
                                selectedEntity = e.get();

                                dragPlaneNormal = glm::vec3(0, 1, 0); // ground plane
                                dragPlaneY = selectedEntity->getTransform().position.y;

                                dragStartPoint = RayPlaneIntersection(
                                    rayOrigin, rayDir,
                                    dragPlaneNormal, dragPlaneY
                                );

                                dragOffset = selectedEntity->getTransform().position - dragStartPoint;


                                if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
                                    confirmSelectedEntity = !confirmSelectedEntity;
                                    confirmedSelectedEntity = selectedEntity;
                                    std::cout << "left clicked to set !\n";
                                }
                            }

                            std::cout << "rtay hitting on object" << std::endl;
                        }
                        else {
                            if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
                                confirmSelectedEntity = !confirmSelectedEntity;
                                confirmedSelectedEntity = nullptr;
                                std::cout << "left clicked to unset !\n";

                            }
                        }
                    }

                    lastMouseX = mouseX;
                    lastMouseY = mouseY;
                }
                // -------------------------
                // STEP 2: Move selected object
                // -------------------------
                else {



                    glm::vec3 currentHit = RayPlaneIntersection(
                        rayOrigin, rayDir,
                        dragPlaneNormal, dragPlaneY
                    );


                    if (SDL_BUTTON(SDL_BUTTON_LEFT)) {

                        selectedEntity->getTransform().position = currentHit + dragOffset;

                    }


                }
            }
            else {

                // Mouse released → stop moving
                selectedEntity = nullptr;
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

            if (event.type == SDL_MOUSEWHEEL) {

                if (event.wheel.y > 0) {
                    if (confirmSelectedEntity && confirmedSelectedEntity != nullptr) {
                        confirmedSelectedEntity->getTransform().scale += glm::vec3(0.1f);

                    }
                    printf("Scroll UP\n");
                }
                else if (event.wheel.y < 0) {
                    if (confirmSelectedEntity && confirmedSelectedEntity != nullptr) {
                        confirmedSelectedEntity->getTransform().scale -= glm::vec3(0.1f);
                    }
                    printf("Scroll DOWN\n");
                }
            }
        }



        handleKeyboardResponse();
        handleMouseResponse();
    }


}