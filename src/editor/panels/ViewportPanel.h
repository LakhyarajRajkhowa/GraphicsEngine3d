#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#define IMGUI_ENABLE_DOCKING
#define IMGUI_ENABLE_DOCKING_EXTENSION
#include "imgui.h"
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "../graphics/camera/Camera3d.h"
#include "../graphics/renderer/Framebuffer.h"

namespace Lengine {

    class ViewportPanel {
    public:
        ViewportPanel();

        void OnImGuiRender(Camera3d& camera);

        // Access framebuffer for rendering the scene
        Framebuffer& GetFramebuffer() { return m_Framebuffer; }

        // Check if the viewport has resized
        bool IsViewportFocused() const { return m_Focused; }
        bool IsViewportHovered() const { return m_Hovered; }

        glm::vec2 GetViewportSize() const { return m_ViewportSize; }
        bool fixCamera = true;

    private:
        Framebuffer m_Framebuffer;

        glm::vec2 m_ViewportSize = { 1280, 720 };
        glm::vec2 m_LastViewportSize = { -1, -1 };

        bool m_Focused = false;  // ImGui window focused
        bool m_Hovered = false;  // Mouse is inside viewport panel
    };

}
