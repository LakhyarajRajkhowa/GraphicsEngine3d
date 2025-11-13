#pragma once

#include "../graphics/geometry/Model.h"
#include "../graphics/geometry/Gizmos.h"
#include "../graphics/renderer/Renderer.h"
#include "../resources/AssetManager.h"
#include "../resources/fileLoader.h"


namespace Lengine {
	class SceneRenderer {
	public:
		SceneRenderer(Camera3d& cam, Scene& scn, AssetManager& assetmgr) :
			camera(cam), scene(scn), assetManager(assetmgr),
			gizmoRenderer(assetmgr, scn, cam) {
		}

		void init();
		void initScene();
		void adjustToScale();
		void clearFrame(const glm::vec4& clearColor);
		void renderScene();
		void endFrame();
		Renderer renderer;

	private:
		Camera3d& camera;
		Scene& scene;
		AssetManager& assetManager;
		GizmoRenderer gizmoRenderer;

		std::vector<GLSLProgram> shaders;
		GLSLProgram defaultShader;
		Model defaultModel;


	};
}