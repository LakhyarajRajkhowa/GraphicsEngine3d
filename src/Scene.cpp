#include "Scene.h"

namespace Lengine {

    Entity* Scene::createEntity(const std::string& name, Mesh* mesh , GLSLProgram* shader ) {
        if (!mesh) mesh = defaultMesh;
        if (!shader) shader = defaultShader;
        auto entity = std::make_unique<Entity>(name, mesh, shader);
        Entity* entityPtr = entity.get();
        entities.push_back(std::move(entity));
        return entityPtr;
    }

    void Scene::removeEntity(const std::string& name) {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [&](const std::unique_ptr<Entity>& e) { return e->name == name; }),
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






    