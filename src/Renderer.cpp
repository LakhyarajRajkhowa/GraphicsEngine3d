#include "Renderer.h"

using namespace Lengine;

void Renderer::renderScene(const Scene& scene, Camera3d& camera) {
    const auto& entities = scene.getEntities();
    for (const auto& entityPtr : entities) {
        Entity* entity = entityPtr.get();
        if (!entity) continue;

        Mesh* mesh = entity->getMesh();
        GLSLProgram* shader = entity->getShader();
       
        if (!mesh || !shader) continue;

        shader->use();

        glm::mat4 model = entity->getTransformMatrix();
     
        shader->setMat4("model", model);
        shader->setMat4("view", camera.getViewMatrix());
        shader->setMat4("projection", camera.getProjectionMatrix());
        shader->setVec3("cameraPos", camera.getCameraPosition());

        mesh->draw();

        shader->unuse();
    }

    
    
}
