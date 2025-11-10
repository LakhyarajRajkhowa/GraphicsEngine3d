#include "InputHandler.h"


namespace Lengine {
    void InputHandler::handleMouseResponse() {



    }

    void InputHandler::handleKeyboardResponse() {

      

        if (inputManager.isKeyDown(SDLK_UP)) {
            if (confirmedSelectedEntity != nullptr)
                confirmedSelectedEntity->getTransform().position.y += 0.01f;
        }
        if (inputManager.isKeyDown(SDLK_DOWN)) {
            if (confirmedSelectedEntity != nullptr)
                confirmedSelectedEntity->getTransform().position.y -= 0.01f;
        }
        if (inputManager.isKeyPressed(SDLK_x)) {
            if (confirmedSelectedEntity != nullptr)
                scene.removeEntity(confirmedSelectedEntity->getName())
                ;
        }
    }

    // this shit is a mess!
    void InputHandler::moveEntity(glm::vec2 mouseCoords) {
        int mouseX = mouseCoords.x, mouseY = mouseCoords.y;

       

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

                if (mouseLeftReleased && confirmedSelectedEntity != nullptr) {
                    confirmedSelectedEntity->isSelected = false;
                    confirmSelectedEntity = false;
                    confirmedSelectedEntity = nullptr;

                }

            }
        }
    }



    void InputHandler::processEvents(const SDL_Event& event) {

        switch (event.type) {
        

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
                            (confirmedSelectedEntity->getTransform().scale.x < 0.1f) ?
                            (confirmedSelectedEntity->getTransform().scale.x < 0.01f) ?
                            glm::vec3(0.001f) : glm::vec3(0.01f) : glm::vec3(0.1f);
                    }
                }
            }

        }
    }

    void InputHandler::handleInputs(
        ImGuiLayer& imguiLayer,
        const ViewportPanel& viewportPanel,
        EditorLayer& editorLayer)
    {

        bool imguiCapturesMouse = imguiLayer.wantsCaptureMouse();
        bool imguiCapturesKeyboard = imguiLayer.wantsCaptureKeyboard();

        bool viewportFocused = viewportPanel.IsViewportFocused();
        bool viewportHovered = viewportPanel.IsViewportHovered();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            // 1. Send all events to ImGui
            imguiLayer.processEvent(event);
            editorLayer.OnEvent(event);
            // Case 1: move camera
            if (camera.isFixed == false) {
                if (viewportFocused)
                    processEvents(event);
                continue;
            }

            // 2. Engine handles only if ImGui does NOT want them
            if (camera.isFixed)
            {
                // ImGui wants event? → give event ONLY to ImGui
                if (imguiCapturesMouse || imguiCapturesKeyboard)
                    continue;

                // Viewport hovered → object selection/manipulation
                if (viewportHovered)
                    processEvents(event);

                // Outside viewport → only UI gets input
                continue;
            }
        }
        // ---- MODE 1: CAMERA MODE ----
        if (!(camera.isFixed))
        {
            if (viewportFocused)
            {
                // camera movement, mouse delta, WASD
                inputManager.updateMouseCoords();
                glm::vec2 mouseCoords = inputManager.getMouseCoords();

                int x, y;
                SDL_GetRelativeMouseState(&x, &y);
                glm::vec2 relativeMouseCoords = { x,y };
                camera.update(0.1f, relativeMouseCoords);
            }

        }


        // ---- MODE 2: EDITOR MODE ----
        if (camera.isFixed)
        {
            if (!imguiCapturesKeyboard && !imguiCapturesMouse)
            {
                if (viewportHovered)
                {
                   

                    inputManager.updateMouseCoords();
                    glm::vec2 mouseCoords = inputManager.getMouseCoords();
                    editorLayer.TrySelectEntity(mouseCoords);
                }
            }

            // outside viewport → nothing for engine
        }
        
        if (camera.isFixed == false)
        {
            // Enter camera freelook mode
            ImGui::SetWindowFocus("Viewport");



            SDL_ShowCursor(SDL_DISABLE);
            SDL_SetRelativeMouseMode(SDL_TRUE);  // lock mouse inside window
        }
        else
        {
            // Enter editor mode
            SDL_SetRelativeMouseMode(SDL_FALSE);
            SDL_ShowCursor(SDL_ENABLE);
        }
        
    }



}