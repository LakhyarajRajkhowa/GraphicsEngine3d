#pragma once

#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Camera3d.h"
#include "AssetManager.h"
#include "Scene.h"
#include "fileLoader.h"

#include "Renderer.h"

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