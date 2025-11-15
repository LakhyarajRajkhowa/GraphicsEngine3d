#pragma once
#include <filesystem>
#include <string>
#include <vector>

#include "../utils/UUID.h"
#include "../resources/AssetManager.h"
namespace Lengine {

    class AssetPanel {
    public:
        AssetPanel(const std::filesystem::path& root, AssetManager& assetMgr);

        void OnImGuiRender();

    private:
        void DrawDirectory(const std::filesystem::path& path);

        void CreateNewFolder(const std::filesystem::path& path);

    private:
        AssetManager& assetManager;

        std::filesystem::path m_RootPath;
        std::filesystem::path m_CurrentPath;

        char m_FolderNameBuffer[256] = "";
        bool m_ShowCreateFolderPopup = false;
    };

}
