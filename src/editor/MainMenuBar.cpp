//#include "MainMenuBar.h"
//
//using namespace Lengine;
//
//void MainMenuBar::DrawMainMenuBar()
//{
//    if (ImGui::BeginMainMenuBar())
//    {
//        if (ImGui::BeginMenu("File"))
//        {
//            if (ImGui::MenuItem("New Project")) { ShowNewProjectDialog(); }
//            if (ImGui::MenuItem("Open Project")) { ShowOpenProjectDialog(); }
//
//
//            ImGui::EndMenu();
//        }
//
//        ImGui::EndMainMenuBar();
//    }
//}
//void MainMenuBar::LoadProject(const fs::path& folder)
//{
//   
//}
//
//void MainMenuBar::ShowOpenProjectDialog()
//{
//    std::string path = FileDialog::OpenFolder("Select Project Folder");
//    if (!path.empty())
//    {
//        LoadProject(path);
//    }
//}
//
//void MainMenuBar::ShowNewProjectDialog()
//{
//    ImGui::OpenPopup("New Project");
//
//    if (ImGui::BeginPopupModal("New Project"))
//    {
//        static char projectName[128] = "NewProject";
//        static char pathBuffer[256] = "C:/Projects/";
//
//        ImGui::InputText("Project Name", projectName, 128);
//        ImGui::InputText("Location", pathBuffer, 256);
//
//        if (ImGui::Button("Create"))
//        {
//            
//            fs::path newFolder = fs::path(pathBuffer) / projectName;
//
//            CreateProjectFolderStructure(newFolder);
//            LoadProject(newFolder);
//
//            ImGui::CloseCurrentPopup();
//        }
//
//        ImGui::SameLine();
//
//        if (ImGui::Button("Cancel"))
//            ImGui::CloseCurrentPopup();
//
//        ImGui::EndPopup();
//    }
//}
//