#include "SceneHeirarchyPanel.h"
using namespace Lengine;
SceneHierarchyPanel::SceneHierarchyPanel(
    Camera3d& cam,
    Scene& scn,
    AssetManager& assetMgr,
    Entity* selectedEntity
)
    :
    camera(cam),
    scene(scn),
    assetManager(assetMgr),
    m_SelectedEntity(selectedEntity)
{
}

void SceneHierarchyPanel::OnImGuiRender() {
    
    // Delete queued entities
    while (!deletedEntityQueue.empty()) {
        scene.removeEntity(deletedEntityQueue.front());
        deletedEntityQueue.pop();
    }
    
    ImGui::Begin("Hierarchy");

    if (ImGui::Button(camera.isFixed ? "Fix Camera: ON" : "Fix Camera: OFF"))
        camera.isFixed = !camera.isFixed;

    ImGui::Separator();
    bool sceneOpen = ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen);
    static bool openModelPopup = false;
    // Right-click popup on Scene node
    if (ImGui::BeginPopupContextItem("SceneContextMenu")) {
        if (ImGui::MenuItem("Add Entity")) {
            ImGui::OpenPopup("Add New Entity");
            openModelPopup = true;
        }
        ImGui::EndPopup();
    }
    if (openModelPopup)
        {
            ImGui::OpenPopup("Add New Entity");
            openModelPopup = false;
        }
    createNewModel();

    if (sceneOpen) {
        for (auto& entity : scene.getEntities()) {

            bool isSelected = entity->isSelected;

            if (ImGui::Selectable(entity->getName().c_str(), isSelected)) {

                // Unselect all other entities
                for (auto& other : scene.getEntities())
                    other->isSelected = false;

                // Select this entity
                entity->isSelected = true;
            }

            if (ImGui::BeginPopupContextItem(entity->getName().c_str())) {
                if (ImGui::MenuItem("Delete")) {
                    deletedEntityQueue.push(entity->getName());
                    entity->isSelected = false;
                }

                

                ImGui::EndPopup();
            }
        }
        ImGui::TreePop();
    }

    ImGui::End();

}



void SceneHierarchyPanel::createNewModel() {
    static char EntityName[128] = "MyEntity";

    if (ImGui::BeginPopupModal("Add New Entity", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter Entity Name:");
        ImGui::InputText("##name", EntityName, IM_ARRAYSIZE(EntityName));
        ImGui::Separator();

        if (ImGui::Button("Create"))
        {
            if (strlen(EntityName) > 0) {

                scene.createEntity(
                    EntityName,
                    UUID(0),
                    assetManager.loadShader(
                        "default",
                        "../assets/Shaders/default.vert",
                        "../assets/Shaders/default.frag"
                    )
                );

                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
