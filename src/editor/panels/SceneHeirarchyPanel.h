#pragma once
#include <imgui.h>
#include <vector>
#include <string>
#include <queue>
#include "../graphics/camera/Camera3d.h"
#include "../scene/Scene.h"
#include "../resources/AssetManager.h"
#include "../external/tinyfiledialogs.h"
namespace Lengine {

    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel(Camera3d& camera, Scene& scene, AssetManager& assetManager, Entity* selectedEntity) ;

        void OnImGuiRender();
        void createNewModel();
        Entity* getSelectedEntity() { return m_SelectedEntity; }
    private:
        Camera3d& camera;
        Scene& scene;
        AssetManager& assetManager;
    private:
        Entity* m_SelectedEntity;
        std::queue<std::string> deletedEntityQueue;

    };

}
