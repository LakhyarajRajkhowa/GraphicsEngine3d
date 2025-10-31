#pragma once

#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Camera3d.h"
#include "AssetManager.h"
#include "Scene.h"
#include "fileLoader.h"

namespace Lengine {
	class UIRenderer {
	public:
		UIRenderer(AssetManager& assetMgr, Scene& scn) : assetManager(assetMgr), scene(scn) {}
		void renderUI(Lengine::Camera3d& camera);
	private:
		AssetManager& assetManager;
		Scene& scene;
		void addImGuiParameters(const char* label);
	};
}
