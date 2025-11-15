#pragma once
#include <imgui.h>
#include "SceneHeirarchyPanel.h"
namespace Lengine {

    class InspectorPanel {
    public:
        InspectorPanel() = default;

        void OnImGuiRender(SceneHierarchyPanel& sceneHeirarchyPanel, AssetManager& assetManager);
        void DrawEntityInspector(Entity* entity, AssetManager& assets);
    private:
        Entity* selectedEntity = nullptr;

    };

}
