#include "Gizmos.h"

using namespace Lengine;

void GizmoRenderer::initGizmoGrid() {
    gizmoGrid = assetManager.getMesh (assetManager.loadMesh("grid", "../assets/obj/plane.obj"));
    gizmoGridShader.compileShaders("../assets/Shaders/grid.vert", "../assets/Shaders/grid.frag");
    gizmoGridShader.linkShaders();

}
void GizmoRenderer::initGizmoSpheres() {

	gizmoSphere = assetManager.getMesh(assetManager.loadMesh("sphere", "../assets/obj/sphere.obj"));
	gizmoSphereShader.compileShaders("../assets/Shaders/boundingSphere.vert", "../assets/Shaders/boundingSphere.frag");
	gizmoSphereShader.linkShaders();

}

void GizmoRenderer::drawGizmoGrid(){
    gizmoGridShader.use();
    gizmoGridShader.setMat4("view", camera.getViewMatrix());
    gizmoGridShader.setMat4("projection", camera.getProjectionMatrix());
    gizmoGridShader.setVec4("color", glm::vec4(0, 1, 0, 0.2));
    glm::mat4 model(1.0f);
    gizmoGridShader.setMat4("model", model);
    gizmoGrid->draw();
    gizmoGridShader.unuse();


}


void GizmoRenderer::drawGizmoSpheres() {
    gizmoSphereShader.use();
    gizmoSphereShader.setMat4("view", camera.getViewMatrix());
    gizmoSphereShader.setMat4("projection", camera.getProjectionMatrix());
    gizmoSphereShader.setVec4("color", glm::vec4(0, 1, 0, 0.2));

    for (auto& e : scene.getEntities()) {
        

        for (auto& sm : assetManager.getMesh(e->getMeshID())->subMeshes) {
            float r = sm.getBoundingRadius();
            glm::vec3 pos = e->getTransform().position;

            glm::mat4 model(1.0f);
            glm::vec3 scaledCenter = sm.getLocalCenter() * e->getTransform().scale;

            model = glm::translate(model, pos + scaledCenter);
            model = glm::scale(model, glm::vec3(r) * e->getTransform().scale);


            gizmoSphereShader.setMat4("model", model);

            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            gizmoSphere->draw();
        }

    }

    gizmoSphereShader.unuse();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


