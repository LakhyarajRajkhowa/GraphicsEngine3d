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

        ImVec2 GetViewportSize() const { return m_ViewportSize; }
        ImVec2 GetViewportPos() const { return m_ViewportPos; }

        ImVec2 ViewportPanel::getMousePosInViewport() const { return mouseInViewport; }
        ImVec2 ViewportPanel::getMousePosInImage() const { return mouseInImage; }

        bool fixCamera = true;

    private:
        Framebuffer m_Framebuffer;
        float offsetValueX = 0.14f;
        float offsetValueY = -0.1f;
        ImVec2 m_ViewportSize = { 1280, 720 };
        ImVec2 m_ViewportPos;
        ImVec2 m_LastViewportSize = { -1, -1 };

        ImVec2 mouseInViewport;
        ImVec2 mouseInImage;


        bool m_Focused = false;  // ImGui window focused
        bool m_Hovered = false;  // Mouse is inside viewport panel
    };

}
