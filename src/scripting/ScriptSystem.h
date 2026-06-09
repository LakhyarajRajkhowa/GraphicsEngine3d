#pragma once

#include "ScriptLibrary.h"
#include "scene/SceneManager.h"
#include "input/InputManager.h"


namespace Lengine {

    class ScriptSystem
    {
    public:
        ScriptSystem(SceneManager& scn, InputManager& input, PhysicsSystem& physics, AssetManager& assets) : sceneManager(scn), input(input) , physics(physics), assets(assets){}

        void Init(const std::string& dllPath);

        void Update(float dt);

        void CheckHotReload();

        void OnCreate();
        void OnUpdate(float dt);
        void OnDestroy();

        void OnCollisionEnter(Entity a, Entity b);
        void OnCollisionExit(Entity a, Entity b);

        const ScriptLibrary& GetLibrary() const { return library; }
        ScriptLibrary& GetLibrary()  { return library; }

        bool IsLibraryLoaded() const { return library.IsLoaded(); }

    private:

        float startTime = 0.0f;
        SceneManager& sceneManager;
        InputManager& input;
        PhysicsSystem& physics;
        AssetManager& assets;

        ScriptLibrary library;
        std::unordered_map<Entity, std::vector<ScriptableEntity*>> ownedScripts;
    };

} // namespace Lengine
