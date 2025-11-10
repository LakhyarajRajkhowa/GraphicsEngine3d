#pragma once

#define IMGUI_ENABLE_DOCKING

#include "imgui.h"
#include "imgui_internal.h"

#include "../scene/Scene.h"
#include "../platform/Window.h"
#include "../editor/panels/ViewportPanel.h"
#include "../editor/panels/SceneHeirarchyPanel.h"
#include "../editor/panels/InspectorPanel.h"
#include "../editor/panels/ConsolePanel.h"

#include "../graphics/geometry/ray.h"
namespace Lengine {

    class EditorLayer {
    public:
        EditorLayer(
            LogBuffer& buffer,
            Scene& scene,
            Camera3d& camera,
            InputManager& inputManager,
            Window& window);
        ~EditorLayer() = default;

        void OnAttach();
        void OnUpdate();       // per-frame logic (optional for future)
        void OnImGuiRender();  // render all panels
        void OnEvent(const SDL_Event& event);
        void OnDetach();

        // Access viewport panel (camera, input needs this)
        ViewportPanel& GetViewportPanel() { return viewportPanel; }
        void TrySelectEntity(const glm::vec2& mouseCoords);

    private:
        void BeginDockspace();
        void SetupDefaultLayout();

        // Editor manipulation
        void HandleDrag(const glm::vec2& mouseCoords);
        void HandleMouseWheel(const SDL_Event& e);
        void HandleKeyboardShortcuts(const SDL_Event& e);



        // Selection state
        bool confirmSelectedEntity = false;
        Entity* confirmedSelectedEntity = nullptr;
        Entity* selectedEntity = nullptr;
        Entity* hoveredEntity = nullptr;

        bool leftMouseDown = false;
        bool leftMouseReleased = false;

        // Dragging
        bool dragActive = false;
        glm::vec3 dragPlaneNormal = glm::vec3(0, 1, 0);
        float dragPlaneY = 0.0f;
        glm::vec3 dragStartPoint;
        glm::vec3 dragOffset;

        bool layoutInitialized = false;
    private:
        // Panels
        ViewportPanel viewportPanel;
        SceneHierarchyPanel hierarchyPanel;
        InspectorPanel inspectorPanel;
        ConsolePanel consolePanel;

        // External engine systems (not owned)
        Scene& scene;
        Camera3d& camera;
        InputManager& inputManager;
        Window& window;
    };

}


