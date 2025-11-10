#pragma once
#include <imgui.h>
#include "../logging/OutputRedirect.h"

namespace Lengine {

    class ConsolePanel {
    public:
        ConsolePanel(LogBuffer& buffer)
            : m_Buffer(buffer) {
        }

        void OnImGuiRender() {
            ImGui::Begin("Console");

            for (auto& msg : m_Buffer.GetLogs()) {
                ImGui::TextUnformatted(msg.c_str());
            }

            ImGui::End();
        }

    private:
        LogBuffer& m_Buffer;
    };

}
