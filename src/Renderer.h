#pragma once
#include "Scene.h"
#include "GLSLProgram.h"
#include "Camera3d.h"

namespace Lengine {
    class Renderer {
    public:
        glm::vec3 sunDir = glm::normalize(glm::vec3(0.2f, -1.0f, 0.3f));
        glm::vec3 sunColor = glm::vec3(1.0f);
        glm::vec3 ambient = glm::vec3(0.03f);
        float shininess = 32.0f;
        float specularStrength = 1.0f;

        void renderScene(const Scene& scene, Camera3d& camera);

    };
}
