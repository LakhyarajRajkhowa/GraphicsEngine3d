#pragma once

#include "../resources/TextureCache.h"

#include "../scene/Entity.h"

namespace Lengine {
    class Scene {
    public:
  
        Entity* createEntity(
            const std::string& name,
            Mesh* mesh = nullptr,
            GLSLProgram* shader = nullptr,
            GLTexture* texture = nullptr
        );

        const  Entity* getEntityByName(const std::string& name) const;
        const std::vector<std::unique_ptr<Entity>>& getEntities() const { return entities; }
        std::vector<std::unique_ptr<Entity>>& Scene::getEntities() {
            return entities;
        }
        void removeEntity(const std::string& name);
        void setDefaults(Mesh* mesh, GLSLProgram* shader) {
            defaultMesh = mesh;
            defaultShader = shader;
        }

        bool showBoundingSphere = false;
    private:
        std::vector<std::unique_ptr<Entity>> entities;
        SubMesh* defaultSubMesh = nullptr;
        Mesh* defaultMesh = nullptr;

        GLSLProgram* defaultShader = nullptr;
    };
}

