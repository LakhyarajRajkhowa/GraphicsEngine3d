#pragma once
#include "../scene/Scene.h"
#include "../resources/AssetManager.h"
namespace Lengine {

    inline Mesh gizmoSphere;
    inline GLSLProgram gizmoShader;

  
    inline void gizmoSphereInit(AssetManager& assetManager) {
       
        gizmoSphere = *assetManager.loadMesh("sphere", "../assets/obj/sphere.obj");   
        gizmoShader.compileShaders("../assets/Shaders/boundingSphere.vert", "../assets/Shaders/boundingSphere.frag");
        gizmoShader.linkShaders();
    }
   

    inline void renderGizmoSpheres( Scene& scene, Camera3d& camera) {
        gizmoShader.use();
        gizmoShader.setMat4("view", camera.getViewMatrix());
        gizmoShader.setMat4("projection", camera.getProjectionMatrix());
        gizmoShader.setVec4("color", glm::vec4(0, 1, 0, 0.2));

        for (auto& e : scene.getEntities()) {
            if (e->getName() == "grid") continue;


            for (auto& sm : e->getMesh()->subMeshes) {
                float r = sm.getBoundingRadius();
                glm::vec3 pos = e->getTransform().position;

                glm::mat4 model(1.0f);
                glm::vec3 scaledCenter = sm.getLocalCenter() * e->getTransform().scale;

                model = glm::translate(model, pos + scaledCenter);
                model = glm::scale(model, glm::vec3(r) * e->getTransform().scale);


                gizmoShader.setMat4("model", model);

                // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                gizmoSphere.draw();
            }
          
        }

       // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

