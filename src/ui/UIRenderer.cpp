//#include "UIRenderer.h"
//namespace Lengine {
//    void UIRenderer::addImGuiParameters(const char* label) {
//
//    }
//
//    void UIRenderer::renderUI(Lengine::Camera3d& camera, Framebuffer& frameBuffer) {
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplSDL2_NewFrame();
//        ImGui::NewFrame();
//        ImGuiWindowFlags flags =
//            ImGuiWindowFlags_NoMove |
//            ImGuiWindowFlags_NoScrollbar |
//            ImGuiWindowFlags_NoScrollWithMouse |
//            ImGuiWindowFlags_NoCollapse |
//            ImGuiWindowFlags_NoInputs;   
//
//        ImGui::Begin("Viewport", nullptr, flags);
//        ImVec2 size(1280, 720);
//        ImGui::Image(
//            (void*)(intptr_t)frameBuffer.GetColorAttachment(),
//            size,
//            ImVec2(0, 1),
//            ImVec2(1, 0)
//        );
//
//        ImGui::End();
//
//
//
//        // ---------------- Camera Position Window ----------------
//        ImGui::SetNextWindowPos(ImVec2(1150, 90));
//        ImGui::SetNextWindowBgAlpha(0.35f);
//        ImGui::Begin("Camera Position", nullptr,
//            ImGuiWindowFlags_NoDecoration |
//            ImGuiWindowFlags_AlwaysAutoResize |
//            ImGuiWindowFlags_NoSavedSettings |
//            ImGuiWindowFlags_NoFocusOnAppearing |
//            ImGuiWindowFlags_NoNav);
//
//        ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)",
//            camera.getCameraPosition().x,
//            camera.getCameraPosition().y,
//            camera.getCameraPosition().z);
//
//        ImGui::End();
//
//        ImGui::Begin("Lighting Settings");
//
//        ImGui::SliderFloat("Ambient Strength", &renderer.ambientStrength, 0.0f, 2.0f);
//
//        ImGui::Separator();
//
//        //ImGui::DragFloat3("Light Position", &renderer.lightPos.x, renderer.lightPos.y, renderer.lightPos.x)
//
//        ImGui::End();
//
//
//        // ---------------- Load File Window ----------------
//        ImGui::SetNextWindowPos(ImVec2(500, 50));
//        ImGui::SetNextWindowBgAlpha(0.35f);
//
//        ImGui::Begin("Tools", nullptr,
//            ImGuiWindowFlags_NoDecoration |
//            ImGuiWindowFlags_AlwaysAutoResize |
//            ImGuiWindowFlags_NoSavedSettings);
//        static bool openModelPopup = false;
//        static char modelName[128] = "MyModel";
//        static char modelPath[512] = "";
//
//        ImGui::SetNextWindowPos(ImVec2(500, 50));
//        ImGui::SetNextWindowBgAlpha(0.35f);
//        ImGui::Begin("Tools", nullptr,
//            ImGuiWindowFlags_NoDecoration |
//            ImGuiWindowFlags_AlwaysAutoResize |
//            ImGuiWindowFlags_NoSavedSettings);
//
//        // --------------------------------------
//        // OPEN POPUP
//        // --------------------------------------
//        if (ImGui::Button("Add Model"))
//        {
//            openModelPopup = true;
//            ImGui::OpenPopup("Add New Model");
//        }
//
//        ImGui::End();
//
//        if (ImGui::Button(scene.showBoundingSphere ? "Hide Bounding Sphere" : "Show Bounding Sphere"))
//        {
//            scene.showBoundingSphere = !scene.showBoundingSphere;
//        }
//
//        // --------------------------------------
//        // POPUP WINDOW
//        // --------------------------------------
//        if (openModelPopup)
//        {
//            ImGui::OpenPopup("Add New Model");
//            openModelPopup = false;
//        }
//
//        if (ImGui::BeginPopupModal("Add New Model", NULL, ImGuiWindowFlags_AlwaysAutoResize))
//        {
//            // ---------------- Model Name ----------------
//            ImGui::Text("Enter Model Name:");
//            ImGui::InputText("##name", modelName, IM_ARRAYSIZE(modelName));
//
//            ImGui::Separator();
//
//            // ---------------- Model File ----------------
//            ImGui::Text("Selected Model:");
//            ImGui::TextWrapped("%s", strlen(modelPath) > 0 ? modelPath : "");
//
//            if (ImGui::Button("Select Model (.obj)"))
//            {
//                const char* filters[] = { "*.obj" };
//                const char* path = tinyfd_openFileDialog(
//                    "Select OBJ File", "", 1, filters, "OBJ Files", 0
//                );
//                if (path) strcpy(modelPath, path);
//            }
//
//            ImGui::Separator();
//
//            // ------------------------------------------------
//            // OPTIONAL TEXTURE
//            // ------------------------------------------------
//            static char texturePath[512] = "";
//            ImGui::Text("Texture (Optional):");
//            ImGui::TextWrapped("%s", strlen(texturePath) > 0 ? texturePath : "");
//
//            if (ImGui::Button("Select Texture (.png)"))
//            {
//                const char* filters[] = { "*.png" };
//                const char* path = tinyfd_openFileDialog(
//                    "Select Texture File", "", 1, filters, "Image Files", 0
//                );
//                if (path) strcpy(texturePath, path);
//            }
//
//            ImGui::Separator();
//
//            // ------------------------------------------------
//            // OPTIONAL SHADER
//            // ------------------------------------------------
//            static char vertShaderPath[512] = "";
//            static char fragShaderPath[512] = "";
//
//            ImGui::Text("Shader (Optional):");
//
//            ImGui::Text("Vertex Shader:");
//            ImGui::TextWrapped("%s", strlen(vertShaderPath) > 0 ? vertShaderPath : "");
//            if (ImGui::Button("Select Vertex Shader (.vert)"))
//            {
//                const char* filters[] = { "*.vert", "*.vs" };
//                const char* path = tinyfd_openFileDialog("Select Vertex Shader", "", 2, filters, "Vertex Shaders", 0);
//                if (path) strcpy(vertShaderPath, path);
//            }
//
//            ImGui::Text("Fragment Shader:");
//            ImGui::TextWrapped("%s", strlen(fragShaderPath) > 0 ? fragShaderPath : "");
//            if (ImGui::Button("Select Fragment Shader (.frag)"))
//            {
//                const char* filters[] = { "*.frag", "*.fs" };
//                const char* path = tinyfd_openFileDialog("Select Fragment Shader", "", 2, filters, "Fragment Shaders", 0);
//                if (path) strcpy(fragShaderPath, path);
//            }
//
//            ImGui::Separator();
//
//            // ------------------------------------------------
//            // CREATE ENTITY
//            // ------------------------------------------------
//            if (ImGui::Button("Create"))
//            {
//                if (strlen(modelName) > 0 && strlen(modelPath) > 0)
//                {
//                    // Load mesh (required)
//                    auto mesh = assetManager.loadMesh(modelName, modelPath);
//
//                    // Load shader (optional)
//                    GLSLProgram* shader;
//                    if (strlen(vertShaderPath) > 0 && strlen(fragShaderPath) > 0)
//                    {
//                        shader = assetManager.loadShader(
//                            modelName,
//                            vertShaderPath,
//                            fragShaderPath
//                        );
//                    }
//                    else
//                    {
//                        shader = assetManager.loadShader(
//                            "default",
//                            "../assets/Shaders/default.vert",
//                            "../assets/Shaders/default.frag"
//                        );
//                    }
//
//                    // Load texture (optional)
//                    GLTexture* texture = {};
//                    if (strlen(texturePath) > 0)
//                    {
//                        texture = assetManager.loadTexture(modelName, texturePath);
//                    }
//
//                    // Create entity
//                    scene.createEntity(modelName, mesh, shader, texture);
//
//                    ImGui::CloseCurrentPopup();
//                }
//                else
//                {
//                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Model name and file required!");
//                }
//            }
//
//            ImGui::SameLine();
//
//            if (ImGui::Button("Cancel"))
//            {
//                ImGui::CloseCurrentPopup();
//            }
//
//            ImGui::EndPopup();
//
//
//        }
//        ImGui::End();
//
//      
//       
//
//    }
//}