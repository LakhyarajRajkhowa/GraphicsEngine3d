#include "SceneRenderer.h"

namespace Lengine {
  
    void SceneRenderer::init() {
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        gizmoRenderer.initGizmo();

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
        
        scene.getEntityByName("sun")->getTransform().position += glm::vec3(20.0f);
        

 
    }
    void SceneRenderer::renderScene() {
        
            gizmoRenderer.drawGizmoGrid();
            renderer.renderScene(scene, camera, assetManager);
        
       

        //gizmoRenderer.drawGizmoSpheres();

    }

    void SceneRenderer::endFrame() {

    }
}
