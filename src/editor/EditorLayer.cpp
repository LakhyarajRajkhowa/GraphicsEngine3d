#include "EditorLayer.h"

namespace Lengine {

    EditorLayer::EditorLayer(
        LogBuffer& buffer,
        Scene& scn,
        Camera3d& cam,
        InputManager& inputMgr,
        Window& win
    )
        : viewportPanel(),     
        hierarchyPanel(),
        inspectorPanel(),
        consolePanel(buffer),
        camera(cam),
        scene(scn),
        inputManager(inputMgr),
        window(win)
    {
    }

    void EditorLayer::OnAttach() {
        // nothing for now
    }

    void EditorLayer::OnDetach() {
        // cleanup if you want later
    }

    void EditorLayer::OnUpdate() {
        // Drag while mouse is held
        if (dragActive && selectedEntity) {
            inputManager.updateMouseCoords();
            glm::vec2 mouseCoords = inputManager.getMouseCoords();
            HandleDrag(mouseCoords);
        }
    }

    void EditorLayer::OnEvent(const SDL_Event& event) {

        switch (event.type) {

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                leftMouseDown = true;
                inputManager.updateMouseCoords();
                TrySelectEntity(inputManager.getMouseCoords());
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                leftMouseDown = false;
                leftMouseReleased = true;
                dragActive = false;
            }
            break;

        case SDL_MOUSEWHEEL:
            HandleMouseWheel(event);
            break;

        case SDL_KEYDOWN:
            HandleKeyboardShortcuts(event);
            break;
        }
    }


    void EditorLayer::TrySelectEntity(const glm::vec2& mouseCoords) {

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
                                if (leftMouseReleased) {
                                    if (selectedEntity != nullptr) {
                                        confirmSelectedEntity = true;
                                        if (confirmedSelectedEntity != nullptr)confirmedSelectedEntity->isSelected = false;
                                        confirmedSelectedEntity = selectedEntity;
                                        confirmedSelectedEntity->isSelected = true;
                                    }
                                    leftMouseReleased = false;
                                }
                            }
                        }

                    }

                }

                if (leftMouseReleased && confirmedSelectedEntity != nullptr) {
                    confirmedSelectedEntity->isSelected = false;
                    confirmSelectedEntity = false;
                    confirmedSelectedEntity = nullptr;

                }

            }
        }
    }
    void EditorLayer::HandleDrag(const glm::vec2& mouseCoords) {

        if (!selectedEntity) return;

        glm::vec3 rayDir = getRayFromMouse(
            mouseCoords.x,
            mouseCoords.y,
            window.getScreenWidth(),
            window.getScreenHeight(),
            camera.getViewMatrix(),
            camera.getProjectionMatrix()
        );
        glm::vec3 rayOrigin = camera.getCameraPosition();

        glm::vec3 hit = RayPlaneIntersection(
            rayOrigin, rayDir,
            dragPlaneNormal, dragPlaneY
        );

        selectedEntity->getTransform().position = hit + dragOffset;
    }
    // -------------------------------------------------------
    // Scale entity using mouse wheel
    // -------------------------------------------------------
    void EditorLayer::HandleMouseWheel(const SDL_Event& e) {

        if (!selectedEntity) return;

        auto& scale = selectedEntity->getTransform().scale;

        if (e.wheel.y > 0) {
            // scale up
            scale += (scale.x < 0.1f) ?
                ((scale.x < 0.01f) ? glm::vec3(0.001f) : glm::vec3(0.01f)) :
                glm::vec3(0.1f);
        }
        else if (e.wheel.y < 0) {
            // scale down
            scale -= (scale.x < 0.1f) ?
                ((scale.x < 0.01f) ? glm::vec3(0.001f) : glm::vec3(0.01f)) :
                glm::vec3(0.1f);
        }
    }

    // -------------------------------------------------------
    // Keyboard shortcuts for entity manipulation
    // -------------------------------------------------------
    void EditorLayer::HandleKeyboardShortcuts(const SDL_Event& e) {

        if (!selectedEntity) return;

        auto& pos = selectedEntity->getTransform().position;

        if (e.key.keysym.sym == SDLK_UP)
            pos.y += 0.01f;

        if (e.key.keysym.sym == SDLK_DOWN)
            pos.y -= 0.01f;

        if (e.key.keysym.sym == SDLK_x) {
            scene.removeEntity(selectedEntity->getName());
            selectedEntity = nullptr;
        }
    }

    void EditorLayer::OnImGuiRender() {

        BeginDockspace();

        // Build layout on first frame
        if (!layoutInitialized) {
           // SetupDefaultLayout(); 
            layoutInitialized = true;
        }

        // ✅ Render panels
        viewportPanel.OnImGuiRender(camera);
        hierarchyPanel.OnImGuiRender(camera);
        inspectorPanel.OnImGuiRender();
        consolePanel.OnImGuiRender();
    }

    // -------------------------------------------------------------
    // Dockspace
    // -------------------------------------------------------------
    void EditorLayer::BeginDockspace() {

        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;

        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("MainDockspace", nullptr, window_flags);

        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MainDockspaceID");
        ImGui::DockSpace(dockspace_id, ImVec2(0, 0));

        ImGui::End();
    }

    // -------------------------------------------------------------
    // Default Layout (Unity-style)
    // -------------------------------------------------------------
    void EditorLayer::SetupDefaultLayout() {

        ImGuiID dockspace_id = ImGui::GetID("MainDockspaceID");

        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

        // Split into 3 columns: Hierarchy | Viewport | Inspector
        ImGuiID dock_left, dock_center, dock_right;

        // Left 20%
        dock_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.20f, nullptr, &dock_center);

        // Right 20%
        dock_right = ImGui::DockBuilderSplitNode(dock_center, ImGuiDir_Right, 0.20f, nullptr, &dock_center);

        // Optional: Bottom Console (25% height)
        ImGuiID dock_bottom;
        dock_bottom = ImGui::DockBuilderSplitNode(dock_center, ImGuiDir_Down, 0.25f, nullptr, &dock_center);

        // Assign windows
        ImGui::DockBuilderDockWindow("Hierarchy", dock_left);
        ImGui::DockBuilderDockWindow("Inspector", dock_right);
        ImGui::DockBuilderDockWindow("Viewport", dock_center);
        ImGui::DockBuilderDockWindow("Console", dock_bottom);

        ImGui::DockBuilderFinish(dockspace_id);
    }

}
