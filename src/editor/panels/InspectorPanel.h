#pragma once
#include <imgui.h>
#include "AssetPanel.h"

#include "../scene/Scene.h"
namespace Lengine {

    class InspectorPanel {
    public:
        InspectorPanel(Scene& scene, AssetManager& assetManager);

        void OnImGuiRender();
        void DrawEntityInspector(Entity* entity, AssetManager& assets);
    private:
        Scene& scene;
        AssetManager& assetManager;
        Entity* selectedEntity = nullptr;

    };

}
