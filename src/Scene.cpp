#include "Scene.h"
#include "TextureCache.h"

namespace Lengine {

    Entity* Scene::createEntity(const std::string& name, Mesh* mesh , GLSLProgram* shader, GLTexture* texture ) {
        if (!mesh) mesh = defaultMesh;
        if (!shader) shader = defaultShader;
        Material* material = new Material(shader);

        material->albedoTexture = texture;
        material->useTexture = true;

        auto entity = std::make_unique<Entity>(name, mesh, material);
        Entity* entityPtr = entity.get();
        entities.push_back(std::move(entity));
        return entityPtr;
    }

    void Scene::removeEntity(const std::string& name) {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [&](const std::unique_ptr<Entity>& e) { return e->getName() == name; }),
            entities.end());
    }

    Entity* Scene::getEntityByName(const std::string& name) {
        for (auto& entity : entities) {
            if (entity->getName() == name) {
                return entity.get();
            }
        }
        return nullptr;
    }
}






    