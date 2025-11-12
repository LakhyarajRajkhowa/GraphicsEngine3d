#pragma once
#include <imgui.h>
#include <vector>
#include <string>

#include "../graphics/camera/Camera3d.h"
namespace Lengine {

    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;

        void OnImGuiRender(Camera3d& camera, Scene& scene) {
            ImGui::Begin("Hierarchy");

            if (ImGui::Button(camera.isFixed ? "Fix Camera: ON" : "Fix Camera: OFF"))
                camera.isFixed = !camera.isFixed;

            ImGui::Separator();
            ImGui::Text("Scene Objects:");
            ImGui::Separator();

            auto& entities = scene.getEntities(); 

            for (auto& entity : entities)
            {
                ImGui::BulletText("%s", entity->getName().c_str());
            }

            ImGui::End();
        }


    };

}
