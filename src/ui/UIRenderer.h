#pragma once

#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "../graphics/camera/Camera3d.h"
#include "../graphics/renderer/Renderer.h"
#include "../resources/fileLoader.h"
#include "../resources/AssetManager.h"
#include "../scene/Scene.h"


namespace Lengine {
	class UIRenderer {
	public:
		UIRenderer(AssetManager& assetMgr, Scene& scn, Renderer& rndr) :
			assetManager(assetMgr), scene(scn), renderer(rndr) {}
		void renderUI(Lengine::Camera3d& camera);
	private:
		AssetManager& assetManager;
		Scene& scene;
		Renderer& renderer;
		void addImGuiParameters(const char* label);
	};
}