#include "Renderer.h"
#include "TextureCache.h"

using namespace Lengine;

void Renderer::renderScene(const Scene& scene, Camera3d& camera) {
    const auto& entities = scene.getEntities();
    for (const auto& entityPtr : entities) {
        Entity* entity = entityPtr.get();
        if (!entity) continue;

        Mesh* mesh = entity->getMesh();
        Material* material = entity->getMaterial();
        if (!mesh || !material || !material->shader) continue;

        GLSLProgram* shader = material->shader;

      

        shader->use();
        // --- Transformation ---
        glm::mat4 model = entity->getTransformMatrix();
        shader->setMat4("model", model);
        shader->setMat4("view", camera.getViewMatrix());
        shader->setMat4("projection", camera.getProjectionMatrix());
        shader->setVec3("cameraPos", camera.getCameraPosition());
        shader->setInt("state", entity->isSelected ? 1 : 0); 

        // --- Material uniforms ---
        material->apply();

        // --- Draw mesh ---
        mesh->draw();

        shader->unuse();
    }

    
    
}
