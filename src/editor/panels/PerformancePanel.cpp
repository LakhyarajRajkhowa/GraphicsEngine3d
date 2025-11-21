#include "PerformancePanel.h"

using namespace Lengine;

PerformancePanel::PerformancePanel() {
	
}

void PerformancePanel::OnImGuiRender() {
    ImGui::Begin("Performance");

    ImGui::Checkbox("Limit FPS", &limitFPS);
    ImGui::SliderInt("Target FPS", &targetFPS, 30, 240); // FPS slider

    FrameStats stats = LimitFPS(targetFPS, limitFPS);
    deltaTime = stats.deltaTime;

    ImGui::Text("FPS: %.1f", stats.fps);
    ImGui::Text("Frame Time: %.2f ms", stats.msPerFrame);
    ImGui::Separator();

    ImGui::End();
}