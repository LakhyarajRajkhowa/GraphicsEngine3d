#pragma once

#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Camera3d.h"

namespace Lengine {
	class UIRenderer {
	public:
		void renderUI(Lengine::Camera3d& camera);
	private:
		void addImGuiParameters(const char* label);
	};
}
