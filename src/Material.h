#pragma once
#include <glm/glm.hpp>
#include <string>
#include "GLSLProgram.h"
#include "GLTexture.h"

namespace Lengine {
    class Material {
    private: 
        glm::vec3 diffuseColor;   // Kd
        glm::vec3 ambientColor;   // Ka
        glm::vec3 specularColor;  // Ks
        float shininess;          // Ns
        std::string diffuseMap;   // map_Kd
        std::string normalMap;    // map_bump
    public:
        GLSLProgram* shader = nullptr;

        // Basic properties
        glm::vec4 baseColor = glm::vec4(0.54, 0.54, 0.54, 1.0);
        float metallic = 0.0f;
        float roughness = 0.0f;
        

        GLTexture* albedoTexture{};
        bool useTexture = false;

        Material(GLSLProgram* shaderProgram)
            : shader(shaderProgram) {
        }

        void apply(GLSLProgram* shader) {

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
