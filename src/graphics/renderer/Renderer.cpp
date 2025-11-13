#include "Renderer.h"

using namespace Lengine;


void Renderer::renderScene(const Scene& scene, Camera3d& camera) {
    const auto& entities = scene.getEntities();
    const Entity* sun = scene.getEntityByName("sun");

    lightPos = sun->getTransform().position;
    for (const auto& entityPtr : entities) {
        Entity* entity = entityPtr.get();
        if (!entity) continue;

        Mesh* mesh = entity->getMesh();
        Material* material = entity->getMaterial();
        if (!mesh || !material || !material->getShader()) continue;

        GLSLProgram* shader = material->getShader();
        shader->use();
        if (loadedEntity.find(entity->getName()) == loadedEntity.end()) {
            for (auto& sm : mesh->subMeshes) {
                float radius = sm.getBoundingRadius();
                if (radius > 1.0f || radius < 0.1f) {
                    float scaleFactor = (1.0f / radius);
                   
                    auto& scale = entity->getTransform().scale;
                    scale *= glm::vec3(scaleFactor);
                    break;
                }

            }
            loadedEntity.insert(entity->getName());
        }
        
        // transforms
        glm::mat4 model = entity->getTransformMatrix();
        shader->setMat4("model", model);
        shader->setMat4("view", camera.getViewMatrix());
        shader->setMat4("projection", camera.getProjectionMatrix());
        shader->setVec3("cameraPos", camera.getCameraPosition());

        // lighting 
        shader->setVec3("lightColor", lightColor);
        shader->setFloat("ambientStrength", ambientStrength);
        shader->setVec3("lightPos", lightPos);
       

        // material 
        material->apply();

        // draw

        
        mesh->draw();

        shader->unuse();
    }
}



////////////