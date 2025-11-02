#include "UIRenderer.h"

namespace Lengine {
    void UIRenderer::addImGuiParameters(const char* label) {

    }

    void UIRenderer::renderUI(Lengine::Camera3d& camera) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // ---------------- Camera Position Window ----------------
        ImGui::SetNextWindowPos(ImVec2(1150, 90));
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("Camera Position", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav);

        ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)",
            camera.getCameraPosition().x,
            camera.getCameraPosition().y,
            camera.getCameraPosition().z);

        ImGui::End();

        // ---------------- Load File Window ----------------
        ImGui::SetNextWindowPos(ImVec2(500, 50));
        ImGui::SetNextWindowBgAlpha(0.35f);

        ImGui::Begin("Tools", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings);
        static bool openModelPopup = false;
        static char modelName[128] = "MyModel";
        static char modelPath[512] = "";

        ImGui::SetNextWindowPos(ImVec2(500, 50));
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("Tools", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings);

        // --------------------------------------
        // OPEN POPUP
        // --------------------------------------
        if (ImGui::Button("Add Model"))
        {
            openModelPopup = true;
            ImGui::OpenPopup("Add New Model");
        }

        ImGui::End();

        if (ImGui::Button(scene.showBoundingSphere ? "Hide Bounding Sphere" : "Show Bounding Sphere"))
        {
            scene.showBoundingSphere = !scene.showBoundingSphere;
        }

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
            ImGui::Text("Enter Model Name:");
            ImGui::InputText("##name", modelName, IM_ARRAYSIZE(modelName));

            ImGui::Separator();

            ImGui::Text("Selected File:");
            ImGui::TextWrapped("%s", (strlen(modelPath) > 0 ? modelPath : "<none>"));

            if (ImGui::Button("Select File"))
            {
                const char* filters[] = { "*.obj" };

                const char* path = tinyfd_openFileDialog(
                    "Select OBJ File", "", 1, filters, "OBJ Files", 0
                );

                if (path)
                {
                    strcpy(modelPath, path);
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Create"))
            {
                if (strlen(modelPath) > 0 && strlen(modelName) > 0)
                {
                    scene.createEntity(
                        modelName,
                        assetManager.loadMesh(modelName, modelPath),
                        assetManager.loadShader("default",
                            "../Shaders/default.vert",
                            "../Shaders/default.frag")
                    );

                    ImGui::CloseCurrentPopup();
                }
                else
                {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Please fill both name and file.");
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        ImGui::End();

      
      

   

    }
}