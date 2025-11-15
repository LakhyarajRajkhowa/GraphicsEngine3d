#include "AssetPanel.h"
#include <imgui.h>

namespace fs = std::filesystem;

namespace Lengine {

    AssetPanel::AssetPanel(const fs::path& root, AssetManager& asstMgr)
        : m_RootPath(root), m_CurrentPath(root),
        assetManager(asstMgr)
    {
    }

    void AssetPanel::OnImGuiRender()
    {
        ImGui::Begin("Assets");

        // Go back (Up one level)
        if (m_CurrentPath != m_RootPath) {
            if (ImGui::Button("<-")) {
                m_CurrentPath = m_CurrentPath.parent_path();
            }
            ImGui::SameLine();
        }

        ImGui::Text("Current: %s", m_CurrentPath.string().c_str());

        ImGui::Separator();

        // Create Folder Button
        if (ImGui::Button("Create Folder")) {
            m_ShowCreateFolderPopup = true;
            strcpy(m_FolderNameBuffer, "");
            ImGui::OpenPopup("CreateFolderPopup");
        }

        // Create Folder Popup
        if (ImGui::BeginPopup("CreateFolderPopup"))
        {
            ImGui::InputText("Folder Name", m_FolderNameBuffer, sizeof(m_FolderNameBuffer));
            if (ImGui::Button("Create"))
            {
                CreateNewFolder(m_CurrentPath / m_FolderNameBuffer);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        // Draw directory content
        DrawDirectory(m_CurrentPath);

        ImGui::End();
    }

    void AssetPanel::DrawDirectory(const fs::path& path)
    {
        for (auto& entry : fs::directory_iterator(path))
        {
            const bool isDir = entry.is_directory();
            const fs::path& filePath = entry.path();
            const std::string name = filePath.filename().string();

            if (isDir)
            {
                // Folder
                ImGui::Text("[DIR] %s", name.c_str());
                if (ImGui::IsItemClicked())
                    m_CurrentPath /= name;
            }
            else
            {
                // ---- FILE ----
                ImGui::Selectable(name.c_str());

                //  DRAG SOURCE FOR MESH FILES
                if (filePath.extension() == ".obj")
                {
                    if (ImGui::BeginDragDropSource())
                    {
                        // Get mesh UUID from asset manager
                        std::string meshName = ExtractNameFromPath(filePath.string());
                        UUID meshID = assetManager.getUUID(meshName);
                        ImGui::SetDragDropPayload("MESH_ASSET", &meshID, sizeof(UUID));
                        ImGui::Text("Mesh: %s", name.c_str());

                        ImGui::EndDragDropSource();
                    }
                }
            }
        }
    }


    void AssetPanel::CreateNewFolder(const fs::path& newFolderPath)
    {
        try {
            if (!fs::exists(newFolderPath))
                fs::create_directory(newFolderPath);
        }
        catch (...) {
            // TODO: log error
        }
    }

}
