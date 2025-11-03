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
            assetManager.loadShader("grid", "../Shaders/grid.vert", "../Shaders/grid.frag"),
            assetManager.loadTexture("grid", "../Textures/brick.png")
        );

        /*scene.createEntity("cube",
            assetManager.loadMesh("cube", "../assets/obj/cube.obj"),
            assetManager.loadShader("cube", "../Shaders/default.vert", "../Shaders/default.frag"),
            assetManager.loadTexture("brick", "../Textures/brick.png")
        );*/

        scene.createEntity("bunny",
            assetManager.loadMesh("bunny", "../assets/obj/bunny2.obj"),
            assetManager.loadShader("bunny", "../Shaders/default.vert", "../Shaders/default.frag"),
            assetManager.loadTexture("bunny", "../Textures/bunny-atlas.png")
        );

        scene.createEntity("buddha",
            assetManager.loadMesh("buddha", "../assets/obj/buddha.obj"),
            assetManager.loadShader("buddha", "../Shaders/default.vert", "../Shaders/default.frag"),
            assetManager.loadTexture("buddha", "../Textures/buddha-atlas.png")
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
