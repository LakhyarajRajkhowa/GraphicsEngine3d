#include "InspectorPanel.h"

using namespace Lengine;

InspectorPanel::InspectorPanel(Scene& scn, AssetManager& asstMgr) :
    scene(scn),
    assetManager(asstMgr) 
{
}

void InspectorPanel::OnImGuiRender() {
    ImGui::Begin("Inspector");

    ImGui::Separator();

    for (auto& entity : scene.getEntities()) {
        if (entity->isSelected) {
            selectedEntity = entity.get();
            break;
        }
        selectedEntity = nullptr;
    }
    
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
    ImGui::Spacing();

    ImGui::Separator();
    ImGui::Text("Transform");
    ImGui::Separator();
    ImGui::Spacing();

    Transform& tr = entity->getTransform();   // or however you access the transform

    // ---------- POSITION ----------
    ImGui::Text("Position");
    ImGui::SameLine();

    if (ImGui::Button("Reset##pos")) {
        tr.position = { 0.0f, 0.0f, 0.0f };
    }
    ImGui::DragFloat3("##Position", glm::value_ptr(tr.position), 0.1f);


    // ---------- ROTATION ----------
    ImGui::Text("Rotation");
    ImGui::SameLine();

    if (ImGui::Button("Reset##rot")) {
        tr.rotation = { 0.0f, 0.0f, 0.0f };
    }
    ImGui::DragFloat3("##Rotation", glm::value_ptr(tr.rotation), 0.5f);


    // ---------- SCALE ----------
    ImGui::Text("Scale");
    ImGui::SameLine();

    if (ImGui::Button("Reset##scale")) {
        tr.scale = { 1.0f, 1.0f, 1.0f };
    }

    // Uniform scale toggle
    static bool uniformScale = true;

    ImGui::Checkbox("Uniform", &uniformScale);

    if (uniformScale)
    {
        float s = tr.scale.x;
        if (ImGui::DragFloat("##UniformScale", &s, 0.05f))
            tr.scale = { s, s, s };
    }
    else
    {
        ImGui::DragFloat3("##Scale", glm::value_ptr(tr.scale), 0.05f);
    }


    

    // ---------------- MESH BLOCK ----------------
    ImGui::Separator();
    ImGui::Text("Mesh");
    ImGui::Separator();
    ImGui::Spacing();
    // --- Draw a centered box ---
    ImGui::BeginGroup();

    // Full row width
    float fullWidth = ImGui::GetContentRegionAvail().x;

    // Box style
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    ImGui::BeginChild("MeshBox", ImVec2(fullWidth, 35.0f), true);

    // Mesh name string
    std::string meshName = "None";
    UUID meshID = entity->getMeshID();

    if (!meshID.isNull()) {
        Mesh* mesh = assets.getMesh(meshID);
        meshName = mesh ? mesh->name : "Invalid Mesh";
    }

    // Center text horizontally
    float textWidth = ImGui::CalcTextSize(meshName.c_str()).x;
    float offset = (fullWidth - textWidth) * 0.5f;
    if (offset > 0) ImGui::SetCursorPosX(offset);

    ImGui::Text("%s", meshName.c_str());

    // --- Drag Drop Target ---
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MESH_ASSET"))
        {
            const MeshDragPayload* data = (const MeshDragPayload*)payload->Data;

            UUID droppedID = data->id;
            std::string meshPath = data->path;

            Mesh* mesh = assetManager.getMesh(droppedID);
            if (!mesh) {
                assetManager.loadMesh2(droppedID, meshPath);
                mesh = assetManager.getMesh(droppedID);
            }

            entity->setMeshID(droppedID);
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::EndChild();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);

    ImGui::EndGroup();

    



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
