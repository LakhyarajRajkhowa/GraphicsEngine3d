#pragma once
#include <glm/glm.hpp>

namespace Lengine {

    struct Particle {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 velocity = glm::vec3(0.0f);

        glm::vec4 colorStart = glm::vec4(1.0f);
        glm::vec4 colorEnd = glm::vec4(1.0f);
        glm::vec4 color = glm::vec4(1.0f); // current, written each Update()

        float sizeStart = 0.1f;
        float sizeEnd = 0.1f;
        float size = 0.1f;   // current

        float rotation = 0.0f;

        float age = 0.0f;
        float lifetime = 1.0f;

        float gravity = -9.8f;
        float drag = 0.0f;

        bool alive = false;

        float NormalizedAge() const {
            return lifetime > 0.0f ? glm::clamp(age / lifetime, 0.0f, 1.0f) : 1.0f;
        }
    };

}