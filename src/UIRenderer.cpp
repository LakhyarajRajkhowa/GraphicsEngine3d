#include "UIRenderer.h"

namespace Lengine {
    void UIRenderer::addImGuiParameters(const char* label) {

    }

    void UIRenderer::renderUI(Lengine::Camera3d& camera) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();



        ImGui::SetNextWindowPos(ImVec2(1150, 90));
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("Camera Position", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav);
        ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", (camera.getCameraPosition().x),
            camera.getCameraPosition().y,
            camera.getCameraPosition().z);
        ImGui::End();



    }
}
