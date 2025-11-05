#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Material.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};


namespace Lengine {
    class SubMesh {
    public:
        SubMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~SubMesh();

        void draw() const;
        glm::vec3 getLocalCenter() { return localCenter; }
        float& getBoundingRadius() { return boundingRadius; }
        void setupMesh();

        bool isSelected = false;
    private:
        void computeBounds();
        

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;


        glm::vec3 localCenter;
        float boundingRadius;

        GLuint VAO, VBO, EBO;
    };

    class Mesh {
    public:
        std::string name;
        std::vector<SubMesh> subMeshes;

        void draw() const {
            for (const auto& sm : subMeshes)
                sm.draw();
        }
    
    };
}

