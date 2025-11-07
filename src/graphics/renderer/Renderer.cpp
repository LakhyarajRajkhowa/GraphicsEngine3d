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



