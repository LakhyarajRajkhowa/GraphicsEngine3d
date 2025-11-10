#pragma once
#include <imgui.h>
#include <vector>
#include <string>

#include "../graphics/camera/Camera3d.h"
namespace Lengine {

    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;

        void OnImGuiRender(Camera3d& camera) {
            ImGui::Begin("Hierarchy");

            if (ImGui::Button(camera.isFixed ? "Fix Camera: ON" : "Fix Camera: OFF"))
            {
                camera.isFixed = !camera.isFixed;
            }
            // Dummy example items
            ImGui::Text("Scene Objects:");
            ImGui::Separator();

            for (int i = 0; i < 10; i++) {
                std::string name = "Entity " + std::to_string(i);
                ImGui::Selectable(name.c_str());
            }

            ImGui::End();
        }
    };

}
