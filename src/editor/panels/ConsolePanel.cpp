#include "ConsolePanel.h"

using namespace Lengine;

void ConsolePanel::OnImGuiRender() {
    ImGui::Begin("Console");

    for (auto& msg : m_Buffer.GetLogs()) {
        ImGui::TextUnformatted(msg.c_str());
    }

    ImGui::End();
}
