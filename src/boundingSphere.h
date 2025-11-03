#pragma once
#include "Mesh.h"

namespace Lengine {

    std::unique_ptr<Mesh> gizmoSphere;
    GLSLProgram gizmoShader;
     inline void generateSphereMesh(std::vector<Vertex>& vertices,
        std::vector<unsigned int>& indices,
        int sectorCount = 24,
        int stackCount = 16)
    {
        const float PI = 3.14159265359f;

        // ----- Generate vertices -----
        for (int i = 0; i <= stackCount; ++i) {
            float stackAngle = PI / 2 - (float)i * (PI / stackCount); // from pi/2 to -pi/2
            float xy = cosf(stackAngle);
            float z = sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j) {
                float sectorAngle = (float)j * (2 * PI / sectorCount);

                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);

                Vertex v{};
                v.position = glm::vec3(x, y, z);
                v.normal = glm::normalize(v.position);
                v.texCoord = glm::vec2((float)j / sectorCount, (float)i / stackCount);

                vertices.push_back(v);
            }
        }

        // ----- Generate indices -----
        int k1, k2;
        for (int i = 0; i < stackCount; ++i) {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }
    inline void boundingSphereInit() {
       

        std::vector<Vertex> verts;
        std::vector<unsigned int> inds;
        generateSphereMesh(verts, inds);
        gizmoSphere = std::make_unique<Mesh>(verts, inds);

        gizmoShader.compileShaders("../Shaders/boundingSphere.vert", "../Shaders/boundingSphere.frag");
        gizmoShader.linkShaders();
    }
   

    inline void renderBoudingSpheres( Scene& scene, Camera3d& camera) {
        gizmoShader.use();
        gizmoShader.setMat4("view", camera.getViewMatrix());
        gizmoShader.setMat4("projection", camera.getProjectionMatrix());
        gizmoShader.setVec4("color", glm::vec4(0, 1, 0, 0.2));

        for (auto& e : scene.getEntities()) {
            if (e->getName() == "grid") continue;

            float r = e->getMesh()->getBoundingRadius();
            glm::vec3 pos = e->getTransform().position;

            glm::mat4 model(1.0f);
            glm::vec3 scaledCenter = e->getMesh()->getLocalCenter() * e->getTransform().scale;

            model = glm::translate(model, pos + scaledCenter);
            model = glm::scale(model, glm::vec3(r) * e->getTransform().scale);


            gizmoShader.setMat4("model", model);

           // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            gizmoSphere->draw();
        }

       // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

