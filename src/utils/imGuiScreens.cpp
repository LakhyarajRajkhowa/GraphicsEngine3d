#include "imGuiScreens.h"

// Actual definitions (exactly 1 time)
std::atomic<bool> isLoading = false;
std::atomic<float> loadingProgress = 0.0f;
std::thread loadingThread;
bool meshReady = true;


void ShowLoadingPopup()
{
    if (isLoading)
        ImGui::OpenPopup("LoadingPopup");

    if (ImGui::BeginPopupModal("LoadingPopup", nullptr,
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDecoration))
    {
        ImGui::Text("Loading Please Wait...");
        ImGui::Spacing();

        ImGui::ProgressBar(loadingProgress, ImVec2(300, 0));

        if (!isLoading)
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

}
