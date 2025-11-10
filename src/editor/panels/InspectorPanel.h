#pragma once
#include <imgui.h>

namespace Lengine {

    class InspectorPanel {
    public:
        InspectorPanel() = default;

        void OnImGuiRender() {
            ImGui::Begin("Inspector");

            ImGui::Text("Inspector");
            ImGui::Separator();

            // Example dummy component values
            static float position[3] = { 0.0f, 1.0f, 0.0f };
            static float rotation[3] = { 0.0f, 0.0f, 0.0f };
            static float scale[3] = { 1.0f, 1.0f, 1.0f };

            ImGui::Text("Transform");
            ImGui::DragFloat3("Position", position, 0.1f);
            ImGui::DragFloat3("Rotation", rotation, 0.1f);
            ImGui::DragFloat3("Scale", scale, 0.1f);

            ImGui::End();
        }
    };

}
