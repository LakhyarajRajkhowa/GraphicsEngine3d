#include "SceneRenderer.h"
#include "boundingSphere.h"

namespace Lengine {
    void SceneRenderer::init() {
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);



    }

    void SceneRenderer::beginFrame(const glm::vec4& clearColor) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SceneRenderer::initScene() {
        scene.createEntity("grid",
            assetManager.loadMesh("grid", "../assets/obj/plane.obj"),
            assetManager.loadShader("grid", "../Shaders/grid.vert", "../Shaders/grid.frag")
        );
        
        boundingSphereInit();

    }
    void SceneRenderer::renderScene() {

        renderer.renderScene(scene, camera);

        if(scene.showBoundingSphere) renderBoudingSpheres(scene, camera);

    }

    void SceneRenderer::endFrame() {

    }
}
