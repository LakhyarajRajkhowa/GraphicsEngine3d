#pragma once
#include <glm/glm.hpp>
#include <string>
#include "GLSLProgram.h"
#include "GLTexture.h"

namespace Lengine {
    class Material {
    public:
        GLSLProgram* shader = nullptr;

        // Basic properties
        glm::vec4 baseColor = glm::vec4(0.54, 0.54, 0.54, 1.0);
        float metallic = 0.5f;
        float roughness = 0.5f;
        

        GLTexture* albedoTexture{};
        bool useTexture = false;

        Material(GLSLProgram* shaderProgram)
            : shader(shaderProgram) {
        }

        void apply() {
            shader->use();

            shader->setFloat("metallic", metallic);
            shader->setFloat("roughness", roughness);
            shader->setVec4("baseColor", baseColor);
            if (useTexture && albedoTexture->id != 0) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, albedoTexture->id);
                shader->setInt("albedoMap", 0);
                shader->setInt("useTexture", 1);
            }
            else {
                shader->setInt("useTexture", 0);
            }
        }
    };
}
