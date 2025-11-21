#pragma once

#include "../resources/TextureCache.h"

#include "../scene/Entity.h"

namespace Lengine {
    class Scene {
    public:
  
        Entity* createEntity(
            const std::string& name,
            UUID meshID
        );

        const  Entity* getEntityByName(const std::string& name) const;
        Entity* getEntityByName(const std::string& name) ;

        const std::vector<std::unique_ptr<Entity>>& getEntities() const { return entities; }
        std::vector<std::unique_ptr<Entity>>& Scene::getEntities() {
            return entities;
        }
        void removeEntity(const std::string& name);
       

        bool showBoundingSphere = false;
        const std::string& getName() const { return name; }
    private:
        std::string name = "scene1";
        std::vector<std::unique_ptr<Entity>> entities;

    };
}

