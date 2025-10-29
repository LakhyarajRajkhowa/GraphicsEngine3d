#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Lengine {
    class Transform {
    public:
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f); 
        glm::vec3 scale = glm::vec3(1.0f);

        glm::mat4 getMatrix() const {
            glm::mat4 model = glm::mat4(1.0f);

            // Apply translation
            model = glm::translate(model, position);

            // Apply rotation (X, Y, Z)
            model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
            model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
            model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));

            // Apply scale
            model = glm::scale(model, scale);

            return model;
        }
    };
}
