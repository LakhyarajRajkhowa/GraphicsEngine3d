#include "InspectorPanel.h"

using namespace Lengine;

void InspectorPanel::OnImGuiRender(SceneHierarchyPanel& sceneHeirarchyPanel, AssetManager& assetManager) {
    ImGui::Begin("Inspector");

    ImGui::Text("Inspector");
    ImGui::Separator();

    selectedEntity = sceneHeirarchyPanel.getSelectedEntity();
    
    DrawEntityInspector(selectedEntity, assetManager);

    ImGui::End();
}

void InspectorPanel::DrawEntityInspector(Entity* entity, AssetManager& assets)
{
    if (!entity) {
        ImGui::Text("No entity selected.");
        return;
    }

    // ---------------- NAME ----------------
    char buffer[256] = {};
    strcpy(buffer, entity->getName().c_str());

    if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
        entity->setName(buffer);
    }

    ImGui::Separator();

    // ---------------- MESH BLOCK ----------------
    ImGui::Text("Mesh:");
    ImGui::SameLine();

    UUID meshID = entity->getMeshID();

    if (meshID.isNull()) {
        ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "None");
    }
    else {
        Mesh* mesh = assets.getMesh(meshID);
        ImGui::Text("%s", mesh ? mesh->name.c_str() : "Invalid Mesh");
    }

    // -------- DRAG AND DROP TARGET --------
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MESH_ASSET"))
        {
            UUID droppedID = *(UUID*)payload->Data;
            std::cout << "meshed dropped : " << droppedID << std::endl;;

            entity->setMeshID(droppedID);
        }
        ImGui::EndDragDropTarget();
    }

    // -------- ADD MESH BUTTON --------
    if (meshID.isNull())
    {
        

        // Drop target under the button too (optional)
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MESH_ASSET"))
            {
                UUID droppedID = *(UUID*)payload->Data;
                entity->setMeshID(droppedID);
            }
            ImGui::EndDragDropTarget();
        }
    }



    // ----------- MATERIALS -----------
    /*
    if (!entity->meshID.isNull()) {
        Mesh* mesh = assets.getMesh(entity->meshID);

        if (mesh) {
            for (size_t i = 0; i < mesh->subMeshes.size(); i++) {

                ImGui::PushID((int)i);
                ImGui::Text("Material %d", (int)i);
                ImGui::SameLine();

                UUID matID = entity->materialIDs[i];
                if (matID.isNull()) {
                    ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "None");
                }
                else {
                    Material* mat = assets.getMaterial(matID);
                    ImGui::Text("%s", mat ? mat->name.c_str() : "Invalid");
                }

                // Drag & drop for materials
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_MATERIAL")) {
                        UUID newMatID = *(UUID*)payload->Data;
                        entity->materialIDs[i] = newMatID;
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::PopID();
            }
        }
        
    }
    */

}
