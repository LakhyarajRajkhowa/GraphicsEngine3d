#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

namespace Lengine {
    class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~Mesh();

        void draw() const;
        glm::vec3 getLocalCenter() { return localCenter; }
        float& getBoundingRadius() { return boundingRadius; }
    private:
        void computeBounds();
        void setupMesh();

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        glm::vec3 localCenter;
        float boundingRadius;

        GLuint VAO, VBO, EBO;
    };
}

