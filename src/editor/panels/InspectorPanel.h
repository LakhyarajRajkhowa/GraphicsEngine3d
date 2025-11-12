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
          //  ImGui::ShowStyleEditor();


            ImGui::End();
        }
    };

}
