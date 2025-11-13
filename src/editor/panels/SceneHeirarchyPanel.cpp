#include "SceneHeirarchyPanel.h"
using namespace Lengine;

void SceneHierarchyPanel::OnImGuiRender(Camera3d& camera, Scene& scene, AssetManager& assetManager) {
    ImGui::Begin("Hierarchy");

    if (ImGui::Button(camera.isFixed ? "Fix Camera: ON" : "Fix Camera: OFF"))
        camera.isFixed = !camera.isFixed;

    ImGui::Separator();
    bool sceneOpen = ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen);
    static bool openModelPopup = false;
    // Right-click popup on Scene node
    if (ImGui::BeginPopupContextItem("SceneContextMenu")) {
        if (ImGui::MenuItem("Add Model")) {
            openModelPopup = true;
            ImGui::OpenPopup("Add New Model");
        }
        ImGui::EndPopup();
    }


    if (sceneOpen) {
        for (auto& entity : scene.getEntities()) {
            bool isSelected = (m_SelectedEntity == entity.get());
            if (ImGui::Selectable(entity->getName().c_str(), isSelected))
                m_SelectedEntity = entity.get();

         
            if (ImGui::BeginPopupContextItem(entity->getName().c_str())) {
                if (ImGui::MenuItem("Delete"))
                    deletedEntityQueue.push(entity->getName());

                if (ImGui::MenuItem("Rename")) {
                    // Rename logic placeholder
                }

                ImGui::EndPopup();
            }
        }
        ImGui::TreePop();
    }

    // Delete queued entities
    while (!deletedEntityQueue.empty()) {
        scene.removeEntity(deletedEntityQueue.front());
        deletedEntityQueue.pop();
    }

    ImGui::End();



    // ---------------- Load File Window ----------------
       

       
        static char modelName[128] = "MyModel";
        static char modelPath[512] = "";
       

        // --------------------------------------
        // POPUP WINDOW
        // --------------------------------------
        if (openModelPopup)
        {
            ImGui::OpenPopup("Add New Model");
            openModelPopup = false;
        }

        if (ImGui::BeginPopupModal("Add New Model", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            // ---------------- Model Name ----------------
            ImGui::Text("Enter Model Name:");
            ImGui::InputText("##name", modelName, IM_ARRAYSIZE(modelName));

            ImGui::Separator();

            // ---------------- Model File ----------------
            ImGui::Text("Selected Model:");
            ImGui::TextWrapped("%s", strlen(modelPath) > 0 ? modelPath : "");

            if (ImGui::Button("Select Model (.obj)"))
            {
                const char* filters[] = { "*.obj" };
                const char* path = tinyfd_openFileDialog(
                    "Select OBJ File", "", 1, filters, "OBJ Files", 0
                );
                if (path) strcpy(modelPath, path);
            }

            ImGui::Separator();

            // ------------------------------------------------
            // OPTIONAL TEXTURE
            // ------------------------------------------------
            static char texturePath[512] = "";
            ImGui::Text("Texture (Optional):");
            ImGui::TextWrapped("%s", strlen(texturePath) > 0 ? texturePath : "");

            if (ImGui::Button("Select Texture (.png)"))
            {
                const char* filters[] = { "*.png" };
                const char* path = tinyfd_openFileDialog(
                    "Select Texture File", "", 1, filters, "Image Files", 0
                );
                if (path) strcpy(texturePath, path);
            }

            ImGui::Separator();

            // ------------------------------------------------
            // OPTIONAL SHADER
            // ------------------------------------------------
            static char vertShaderPath[512] = "";
            static char fragShaderPath[512] = "";

            ImGui::Text("Shader (Optional):");

            ImGui::Text("Vertex Shader:");
            ImGui::TextWrapped("%s", strlen(vertShaderPath) > 0 ? vertShaderPath : "");
            if (ImGui::Button("Select Vertex Shader (.vert)"))
            {
                const char* filters[] = { "*.vert", "*.vs" };
                const char* path = tinyfd_openFileDialog("Select Vertex Shader", "", 2, filters, "Vertex Shaders", 0);
                if (path) strcpy(vertShaderPath, path);
            }

            ImGui::Text("Fragment Shader:");
            ImGui::TextWrapped("%s", strlen(fragShaderPath) > 0 ? fragShaderPath : "");
            if (ImGui::Button("Select Fragment Shader (.frag)"))
            {
                const char* filters[] = { "*.frag", "*.fs" };
                const char* path = tinyfd_openFileDialog("Select Fragment Shader", "", 2, filters, "Fragment Shaders", 0);
                if (path) strcpy(fragShaderPath, path);
            }

            ImGui::Separator();

            // ------------------------------------------------
            // CREATE ENTITY
            // ------------------------------------------------
            if (ImGui::Button("Create"))
            {
                if (strlen(modelName) > 0 && strlen(modelPath) > 0)
                {
                    // Load mesh (required)
                    auto mesh = assetManager.loadMesh(modelName, modelPath);

                    // Load shader (optional)
                    GLSLProgram* shader;
                    if (strlen(vertShaderPath) > 0 && strlen(fragShaderPath) > 0)
                    {
                        shader = assetManager.loadShader(
                            modelName,
                            vertShaderPath,
                            fragShaderPath
                        );
                    }
                    else
                    {
                        shader = assetManager.loadShader(
                            "default",
                            "../assets/Shaders/default.vert",
                            "../assets/Shaders/default.frag"
                        );
                    }

                    // Load texture (optional)
                    GLTexture* texture = {};
                    if (strlen(texturePath) > 0)
                    {
                        texture = assetManager.loadTexture(modelName, texturePath);
                    }

                    // Create entity
                    scene.createEntity(modelName, mesh, shader, texture);

                    ImGui::CloseCurrentPopup();
                }
                else
                {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Model name and file required!");
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
