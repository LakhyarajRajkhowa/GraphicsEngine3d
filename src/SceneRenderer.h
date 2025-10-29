#pragma once

#include "Model.h"
#include "Renderer.h"

namespace Lengine {
	class SceneRenderer {
	public:
		SceneRenderer(Lengine::Camera3d& cam): camera(cam) {}

		void init();
		void initScene();
		void beginFrame(const glm::vec4& clearColor);
		void renderScene();
		void endFrame();
	private:
		Camera3d& camera;
		Scene scene;
		Renderer renderer;
		GLSLProgram defaultShader;
		Model defaultModel;
		std::shared_ptr<Mesh> defaultMesh;


		GLSLProgram gridShader;
		std::shared_ptr<Mesh> gridMesh;
		Model gridModel;
	};
}