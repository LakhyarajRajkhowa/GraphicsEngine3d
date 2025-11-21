#include "SceneRenderer.h"

namespace Lengine {
  
    void SceneRenderer::init() {
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        assetManager.LoadAllMetaFiles("../assets");

        gizmoRenderer.initGizmo();
        
    }

    void SceneRenderer::clearFrame(const glm::vec4& clearColor) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SceneRenderer::initScene() {
       
        scene.createEntity("cube",
            UUID(3190633498160195690) // uuid of cube
        );
        
        scene.createEntity("sun",
          UUID(9343755805681254094) // uuid of sphere
        );
        
        scene.getEntityByName("sun")->getTransform().position += glm::vec3(20.0f);
        
 
    }
    void SceneRenderer::renderScene() {
        double ms = ImGui::GetTime();
        double s = ms / 1.0f;
        if (renderer.changeColor) {
            renderer.lightColor.x = sin(s * 2.0f);
            renderer.lightColor.y = sin(s * 0.7f);
            renderer.lightColor.z = sin(s * 1.3f);
        }       

        gizmoRenderer.drawGizmoGrid();
        renderer.renderScene(scene, camera, assetManager);
        gizmoRenderer.drawGizmoSpheres();

    }

    void SceneRenderer::endFrame() {

    }
}
