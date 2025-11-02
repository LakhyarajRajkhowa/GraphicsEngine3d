#pragma once

#include "Entity.h"

namespace Lengine {
    class Scene {
    public:
        
        Entity* createEntity(const std::string& name, Mesh* mesh = nullptr, GLSLProgram* shader = nullptr);
        Entity* getEntityByName(const std::string& name);
        const std::vector<std::unique_ptr<Entity>>& getEntities() const { return entities; }
        void removeEntity(const std::string& name);
        void setDefaults(Mesh* mesh, GLSLProgram* shader) {
            defaultMesh = mesh;
            defaultShader = shader;
        }

        bool showBoundingSphere = false;

    private:
        std::vector<std::unique_ptr<Entity>> entities;
        Mesh* defaultMesh = nullptr;
        GLSLProgram* defaultShader = nullptr;
    };
}

