#include "ImGuiLayer.h"


using namespace Lengine;
void ImGuiLayer::init(SDL_Window* window, SDL_GLContext glContext) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

    
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 330");

    
}


void ImGuiLayer::shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}


void ImGuiLayer::processEvent(const SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        inputManager.pressKey(event.key.keysym.sym);
        break;
    case SDL_KEYUP:
        inputManager.releaseKey(event.key.keysym.sym);
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
            /* TODO */
        break;

    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT) {
           /* TODO */
        }
        break;
   
    }
}


void ImGuiLayer::beginFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}


void ImGuiLayer::endFrame() {
   
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

}


bool ImGuiLayer::wantsCaptureMouse() const {
	return ImGui::GetIO().WantCaptureMouse;
}


bool ImGuiLayer::wantsCaptureKeyboard() const {
	return ImGui::GetIO().WantCaptureKeyboard;
}