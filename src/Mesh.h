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

    private:
        void setupMesh();

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        GLuint VAO, VBO, EBO;
    };
}

