#pragma once

#include <queue>

#include "resources/TextureCache.h"

#include "scene/Entity.h"
#include "scene/ECSRegistry.h"

#include "assets/MaterialRegistry.h"

#include "transform/TransformSystem.h"

#include "utils/miscellaneous.h"


namespace Lengine {
    class Scene {
    public:
        
        Scene(const std::string& n, UUID sID)
            : name(n), sceneID(sID) 
        {
        }
  
        Entity CreateEntity(const std::string& name = GenerateRandomString(16));
        Entity CreateEntity_root(const std::string& name);
        Entity CopyEntity(const Entity originalEntityId, Entity entityId = NullEntity);
        Entity DuplicateHierarchy(Entity rootID);

        void DestroyEntity(Entity entity);
        
         Entity GetRootParent(const Entity& entityID);
         Entity GetParent(const Entity& entityID);


       
        const std::vector<Entity>& getEntities() const { return registry.GetEntities(); }
        std::vector<Entity>& getEntities() {
            return registry.GetEntities();
        }

        const std::vector<Entity>& GetRootEntities() const { return rootEntities; }
        std::vector<Entity>& GetRootEntities() {
            return rootEntities;
        }

        bool IsRootEntity(const Entity id)
        {
            const auto& roots = GetRootEntities();
            return std::find(roots.begin(), roots.end(), id) != roots.end();
        }


        std::string getName() { return name; }
        const std::string& getName() const { return name; }
        void rename(const std::string newName) { name = newName; }

        UUID getUUID() const { return sceneID; }

        bool HasChildren(Entity entityID) const;
        const std::vector<Entity>& GetChildren(Entity entityID) const;

        void SetParent(Entity child, Entity parent);
        void MakeOrphan(Entity child);

        std::unique_ptr<Scene> Clone();
       
        // Scene.h — just this
        const Registry& GetRegistry() const { return registry; }
        Registry& GetRegistry() { return registry; }

        const Entity& GetDirectionalShadowCaster() const {
            return directionalShadowCaster;
        }

        void SetDirectionalShadowCaster(Entity entity)
        {
            // Clear old one
            if (directionalShadowCaster != UUID::Null &&
                registry.lights.Has(directionalShadowCaster))
            {
                registry.lights.Get(directionalShadowCaster).castShadow = false;
            }

            directionalShadowCaster = entity;

            // Set new one
            if (directionalShadowCaster != UUID::Null &&
                registry.lights.Has(directionalShadowCaster))
            {
                registry.lights.Get(directionalShadowCaster).castShadow = true;
            }
        }

        const Entity& GetPointShadowCaster() const {
            return pointShadowCaster;
        }

        void SetPointShadowCaster(Entity entity)
        {
            if (pointShadowCaster != NullEntity &&
                registry.lights.Has(pointShadowCaster))
            {
                registry.lights.Get(pointShadowCaster).castShadow = false;
            }

            pointShadowCaster = entity;

            if (pointShadowCaster != NullEntity &&
                registry.lights.Has(pointShadowCaster))
            {
                registry.lights.Get(pointShadowCaster).castShadow = true;
            }
        }

        const Entity& GetPrimaryCamera() const {
            return primaryCamera;
        }

        void SetPrimaryCamera(const Entity& id) {
            primaryCamera = id;
        }
        
        std::string GenerateDuplicateName(Scene* scene, const std::string& baseName);

        const Entity GetNextEntityID() { return nextEntityID; }

        void Update();



        // Do not use this inside runtime Entity loops
        Entity Scene::createEntity_root(const std::string& name)
        {
            Entity id = nextEntityID++;

            registry.createEntity();
            registry.nameTags.Add(id, NameTagComponent(name));

            rootEntities.push_back(id);

            return id;
        }

        // Scene.cpp
        Entity Scene::CreateEntityImmediate(std::string name = GenerateRandomString(16))
        {
            Entity id = nextEntityID++;

            // Register in the entity list and root list directly
            registry.GetEntities().push_back(id);
            rootEntities.push_back(id);

            if (!name.empty())
                registry.nameTags.Add(id, NameTagComponent(name));

            // Sync registry's counter so it never collides
            registry.SyncNextEntityID(nextEntityID);

            return id;
        }


    private:
        std::string name;
        UUID sceneID;
        std::vector<Entity> rootEntities;

        Entity nextEntityID = 1;
        Entity primaryCamera = NullEntity;
        Entity directionalShadowCaster = NullEntity;
        Entity pointShadowCaster = NullEntity;
        
        Registry registry;

        std::queue<std::pair<Entity, std::string>> entityCreateQueue;
        std::queue<std::pair<Entity, std::string>> rootEntityCreateQueue;
        std::queue<std::pair<Entity, Entity>> copyEntityQueue;
        std::queue<Entity> duplicateHierarchyQueue;
        std::queue<Entity> destroyEntityQueue;

        Entity copyEntity(const Entity originalEntityId, Entity entityId);

        Entity duplicateHierarchyImmediate(
            Entity originalID,
            Entity newParent = NullEntity,
            Entity newRoot = NullEntity
        );

        Entity copyEntityImmediate(
            Entity entityID,
            Entity originalEntityID
        );

        void removeEntityImmediate(const Entity);
        void removeEntityRecursiveImmediate(Entity id);
        void removeEntityRecursive(Entity id);



    };
}

