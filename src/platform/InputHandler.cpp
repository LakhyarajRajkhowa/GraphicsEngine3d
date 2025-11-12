#include "InputHandler.h"
#include <algorithm>

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

        }


        // ---- MODE 2: EDITOR MODE ----
        if (camera.isFixed)
        {
            if (viewportHovered)
            {
                

                for (SDL_Keycode key : { SDLK_UP, SDLK_DOWN, SDLK_x}) {
                    editorLayer.HandleKeyboardShortcuts(key);
                }

                for ( Uint8 button : { SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT, SDL_BUTTON_MIDDLE}) {
                    if (inputManager.isMouseButtonPressed(button)) {
                        switch (button) {
                        case  SDL_BUTTON_LEFT:
                            editorLayer.selectHoveredEntity();
                            break;
                        }
                    }
                    
                    if (inputManager.isMouseButtonDown(button)) {
                        switch (button) {
                        case  SDL_BUTTON_LEFT: 
                            editorLayer.HandleDrag();
                            break;
                        }
                    }           
                }

                if (inputManager.getScrollY()) {
                    editorLayer.HandleMouseWheel(inputManager.getScrollY());
                }
                inputManager.resetScroll();

            } 
        }
      
        
        if (camera.isFixed == false)
        {
            // Enter camera freelook mode
            ImGui::SetWindowFocus("Viewport");

            SDL_ShowCursor(SDL_DISABLE);
            SDL_SetRelativeMouseMode(SDL_TRUE);  // lock mouse inside window


            if (viewportFocused)
            {
                ImVec2 pos = editorLayer.GetViewportPanel().GetViewportPos();        // top-left corner
                ImVec2 size = editorLayer.GetViewportPanel().GetViewportSize();      // size
                int mx, my;
                SDL_GetRelativeMouseState(&mx, &my);
                int clampedX = std::clamp(mx, (int)pos.x, (int)(pos.x + size.x - 1));
                int clampedY = std::clamp(my, (int)pos.y, (int)(pos.y + size.y - 1));

                if (mx != clampedX || my != clampedY)
                    SDL_WarpMouseInWindow(window.getWindow(), clampedX, clampedY);

                glm::vec2 relativeMouseCoords = { mx,my };
                camera.update(0.1f, relativeMouseCoords);

                
            }
        }
        else
        {
            // Enter editor mode
            SDL_SetRelativeMouseMode(SDL_FALSE);
            SDL_ShowCursor(SDL_ENABLE);
        }
        
    }



}