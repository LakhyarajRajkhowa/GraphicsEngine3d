//#pragma once
//#include <imgui.h>
//#include <fstream>
//#include <filesystem>
//
//namespace fs = std::filesystem;
//namespace Lengine {
//    class MainMenuBar {
//    public:
//        void DrawMainMenuBar()
//        {
//            if (ImGui::BeginMainMenuBar())
//            {
//                if (ImGui::BeginMenu("File"))
//                {
//                    if (ImGui::MenuItem("New Project")) { ShowNewProjectDialog(); }
//                    if (ImGui::MenuItem("Open Project")) { ShowOpenProjectDialog(); }
//                   
//
//                    ImGui::EndMenu();
//                }
//              
//                ImGui::EndMainMenuBar();
//            }
//        }
//    private:
//        void LoadProject(const fs::path& folder);
//        void ShowNewProjectDialog();
//        void ShowOpenProjectDialog();
//
//    };
//}
//