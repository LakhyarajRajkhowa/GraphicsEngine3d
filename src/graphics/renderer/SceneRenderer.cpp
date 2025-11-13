#include "SceneRenderer.h"

namespace Lengine {
    void SceneRenderer::adjustToScale() {
        for (auto& entity : scene.getEntities()) {
            for (auto& sm : entity.get()->getMesh()->subMeshes) {
                float radius = sm.getBoundingRadius();
                if (radius > 1.0f || radius < 0.1f) {
                    float scaleFactor = (1.0f / radius);
                    std::cout << "entity radius: " << radius << std::endl;
                    std::cout << "scale factor: " << scaleFactor << std::endl;
                    glm::vec3 scale = entity->getTransform().scale;
                    scale *= glm::vec3(scaleFactor);
                    break;
                }

            }
        }
        
    }
    void SceneRenderer::init() {
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        gizmoRenderer.initGizmo();
        adjustToScale();

    }

    void SceneRenderer::clearFrame(const glm::vec4& clearColor) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SceneRenderer::initScene() {
     
        scene.createEntity("cube",
            assetManager.loadMesh("cube", "../assets/obj/cube.obj"),
            assetManager.loadShader("cube", "../assets/Shaders/default.vert", "../assets/Shaders/default.frag")
        );

        scene.createEntity("sun",
            assetManager.loadMesh("sun", "../assets/obj/sphere.obj"),
            assetManager.loadShader("sun", "../assets/Shaders/lightSource.vert", "../assets/Shaders/lightSource.frag")

        );
   
 
    }
    void SceneRenderer::renderScene() {

        gizmoRenderer.drawGizmoGrid();
        renderer.renderScene(scene, camera);

      
        //gizmoRenderer.drawGizmoSpheres();

    }

    void SceneRenderer::endFrame() {

    }
}
