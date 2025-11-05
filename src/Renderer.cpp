#include "Renderer.h"
#include "TextureCache.h"

using namespace Lengine;


void Renderer::renderScene(const Scene& scene, Camera3d& camera) {
    const auto& entities = scene.getEntities();

    // compute global light once per pass (directional)
   

    for (const auto& entityPtr : entities) {
        Entity* entity = entityPtr.get();
        if (!entity) continue;

        Mesh* mesh = entity->getMesh();
        Material* material = entity->getMaterial();
        if (!mesh || !material || !material->shader) continue;

        GLSLProgram* shader = material->shader;
        shader->use();

        // transforms
        glm::mat4 model = entity->getTransformMatrix();
        shader->setMat4("model", model);
        shader->setMat4("view", camera.getViewMatrix());
        shader->setMat4("projection", camera.getProjectionMatrix());
        shader->setVec3("cameraPos", camera.getCameraPosition());

        // lighting uniforms (global for this pass)
        shader->setVec3("lightDir", glm::normalize(sunDir));
        shader->setVec3("lightColor", sunColor);
        shader->setVec3("ambient", ambient);
        shader->setFloat("shininess", shininess);
        shader->setFloat("specularStrength", specularStrength);

        std::cout << "shininess :" << shininess << "\n";

        // material -> set material-specific uniforms + bind textures
        material->apply(shader);

        // draw
        mesh->draw();

        shader->unuse();
    }
}



