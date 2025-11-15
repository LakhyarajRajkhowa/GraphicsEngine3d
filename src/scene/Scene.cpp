#include "Scene.h"

namespace Lengine {

  

    Entity* Scene::createEntity(const std::string& name, UUID meshID, GLSLProgram* shader, GLTexture* texture) {
       
        if (!shader) shader = defaultShader;


        Material* material = new Material(shader);
        if (texture) {
            material->albedoTexture = texture;
            material->useTexture = true;
        }


        auto entity = std::make_unique<Entity>(name, meshID, material);
        Entity* entityPtr = entity.get();
        entities.push_back(std::move(entity));

        

        return entityPtr;
    }

    void Scene::removeEntity(const std::string& name) {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [&](const std::unique_ptr<Entity>& e) { return e->getName() == name; }),
            entities.end());
    }

    const Entity* Scene::getEntityByName(const std::string& name) const {
        for (auto& entity : entities) {
            if (entity->getName() == name) {
                return entity.get();
            }
        }
        return nullptr;
    }
    Entity* Scene::getEntityByName(const std::string& name)  {
        for (auto& entity : entities) {
            if (entity->getName() == name) {
                return entity.get();
            }
        }
        return nullptr;
    }
}






    //////////////////