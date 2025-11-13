#pragma once
#include "../scene/Scene.h"
#include "../graphics/opengl/GLSLProgram.h"
#include "../graphics/camera/Camera3d.h"
#include "../resources/TextureCache.h"

namespace Lengine {
    class Renderer {
    public:
        glm::vec3 lightColor = glm::vec3(1.0f);
        glm::vec3 lightPos;
        float ambientStrength = 0.1f;

        void renderScene(const Scene& scene, Camera3d& camera);

        std::unordered_set<std::string> loadedEntity;
    };
}
