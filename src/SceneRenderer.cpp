#include "SceneRenderer.h"

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
        // Compile shader
        defaultShader.compileShaders("../../Shaders/default.vert", "../../Shaders/default.frag");
        defaultShader.linkShaders();


        gridShader.compileShaders("../../Shaders/grid.vert", "../../Shaders/grid.frag");
        gridShader.linkShaders();

        // Load cube model (or plane)
        defaultModel.loadModel("../../assets/obj/cube.obj", defaultMesh);

        
        gridModel.loadModel("../../assets/obj/plane.obj", gridMesh);

        // Create entities
        scene.setDefaults(defaultMesh.get(), &defaultShader);
        scene.createEntity("Cube1");
        scene.createEntity("grid", gridMesh.get(), &gridShader);
    }
    void SceneRenderer::renderScene() {

        renderer.renderScene(scene, camera);

    }

    void SceneRenderer::endFrame() {

    }
}
