#include "Mesh.h"
#include <iostream>

namespace Lengine {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
        : vertices(vertices), indices(indices)
    {
        computeBounds();
        setupMesh();

    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Mesh::computeBounds() {
        if (vertices.empty()) return;

        glm::vec3 minV = vertices[0].position;
        glm::vec3 maxV = vertices[0].position;

        // Step 1: find bounding box
        for (const auto& v : vertices) {
            minV = glm::min(minV, v.position);
            maxV = glm::max(maxV, v.position);
        }

        // Step 2: center = middle of min and max
        localCenter = (minV + maxV) * 0.5f;

        // Step 3: bounding radius = max distance from center
        float maxDist = 0.0f;
        for (const auto& v : vertices) {
            maxDist = glm::max(maxDist, glm::length(v.position - localCenter));
        }

        boundingRadius = maxDist;
    }

    void Mesh::setupMesh() {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
            vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
            indices.data(), GL_STATIC_DRAW);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // TexCoord
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        glBindVertexArray(0);

    }

    void Mesh::draw() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

}
