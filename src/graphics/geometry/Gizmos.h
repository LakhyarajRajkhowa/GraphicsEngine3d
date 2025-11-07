#pragma once

#include "../resources/AssetManager.h"
#include "../scene/Scene.h"
namespace Lengine {
	class GizmoRenderer {
	public:
		GizmoRenderer(AssetManager& asstmgr, Scene& scn, Camera3d& cam) :
			assetManager(asstmgr), scene(scn), camera(cam)
		{
			
		}
		void initGizmo() {
			initGizmoGrid();
			initGizmoSpheres();
		}
		void drawGizmoSpheres();
		void drawGizmoGrid();	

	private:
		AssetManager& assetManager;
		Scene& scene;
		Camera3d& camera;

		Mesh* gizmoSphere = nullptr;
		GLSLProgram gizmoSphereShader;

		Mesh* gizmoGrid = nullptr;
		GLSLProgram gizmoGridShader;


		void initGizmoSpheres();
		void initGizmoGrid();
	};
}