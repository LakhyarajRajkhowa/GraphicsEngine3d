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



   
    
    void InputHandler::handleInputs(
        ImGuiLayer& imguiLayer,
        EditorLayer& editorLayer)
    {

        bool imguiCapturesMouse = imguiLayer.wantsCaptureMouse();
        bool imguiCapturesKeyboard = imguiLayer.wantsCaptureKeyboard();

        bool viewportFocused = editorLayer.GetViewportPanel().IsViewportFocused();
        bool viewportHovered = editorLayer.GetViewportPanel().IsViewportHovered();

        const float dt = editorLayer.GetPerformancePanel().getDeltaTime();
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
      
        
        if (camera.isFixed == false )
        {


        
            if(viewportFocused)
            {
                

                // Enter camera freelook mode
                ImGui::SetWindowFocus("Viewport");
                SDL_ShowCursor(SDL_DISABLE);
                SDL_SetRelativeMouseMode(SDL_TRUE);  

                ImVec2 pos = editorLayer.GetViewportPanel().GetViewportPos();        
                ImVec2 size = editorLayer.GetViewportPanel().GetViewportSize();     
                int mx, my;
                SDL_GetRelativeMouseState(&mx, &my);
                int clampedX = std::clamp(mx, (int)pos.x, (int)(pos.x + size.x - 1));
                int clampedY = std::clamp(my, (int)pos.y, (int)(pos.y + size.y - 1));

                if (mx != clampedX || my != clampedY)
                    SDL_WarpMouseInWindow(window.getWindow(), clampedX, clampedY);

                glm::vec2 relativeMouseCoords = { mx,my };
                camera.update(dt, relativeMouseCoords);

            }
            else {
                SDL_ShowCursor(SDL_DISABLE);
                SDL_SetRelativeMouseMode(SDL_TRUE);
                int mx, my;
                SDL_GetRelativeMouseState(&mx, &my);
                glm::vec2 relativeMouseCoords = { mx,my };
                camera.update(dt, relativeMouseCoords);
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