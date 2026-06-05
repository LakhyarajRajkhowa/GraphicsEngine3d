#pragma once

#include "../scene/Scene.h"
#include "../resources/AssetManager.h"
#include "physics/PhysicsSystem.h"

namespace Lengine {

    class SceneManager {
    private:

        std::vector<std::unique_ptr<Scene>> scenes;

        Scene* editorScene = nullptr;
        std::unique_ptr<Scene> runtimeScene = nullptr;

        AssetManager& assetManager;
        PhysicsSystem& physics;

    public:

        SceneManager(AssetManager& asstMgr, PhysicsSystem& physics)
            : assetManager(asstMgr), physics(physics) {}

        Scene* GetActiveScene(EditorMode mode)
        {
            return (mode == EditorMode::EDIT)
                ? editorScene
                : runtimeScene.get();
        }

        Scene* GetEditorScene()
        {
            return editorScene;
        }

        std::unique_ptr<Scene>& GetRuntimeScene()
        {
            return runtimeScene;
        }

        void CreateRuntimeScene()
        {

            runtimeScene = editorScene->Clone();
            

        }

        void setActiveScene(Scene* scene) {
            if (!scene)
                return;

            editorScene = scene;
        }

        const auto& getScenes() const
        {
            return scenes;
        }

        auto& getScenes()
        {
            return scenes;
        }

        void loadScenes(const std::vector<std::string>& scenes);
    };

}