#pragma once
#include <glm/glm.hpp>

namespace Lengine {
    inline glm::vec3 getRayFromMouse(
        float mouseX,
        float mouseY,
        int screenWidth,
        int screenHeight,
        const glm::mat4& view,
        const glm::mat4& projection
    ) {
        // Convert to normalized device coordinates (-1 to 1)
        float x = (2.0f * mouseX) / screenWidth - 1.0f;
        float y = 1.0f - (2.0f * mouseY) / screenHeight;
        glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
        glm::vec4 rayEye = glm::inverse(projection) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
        glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));

        return rayWorld;
    }

    inline bool rayIntersectsSphere(
        const glm::vec3& rayOrigin,
        const glm::vec3& rayDir,
        const glm::vec3& center, 
        float radius
    ) {
        glm::vec3 oc = rayOrigin - center;
        float b = glm::dot(oc, rayDir);
        float c = glm::dot(oc, oc) - radius * radius;
        float h = b * b - c;
        return h >= 0.0f;
    }

    inline glm::vec3 RayPlaneIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDir,
        glm::vec3 planeNormal, float planeHeight)
    {
        float denom = glm::dot(rayDir, planeNormal);
        if (fabs(denom) < 0.0001f) return rayOrigin; // parallel

        float t = (planeHeight - glm::dot(rayOrigin, planeNormal)) / denom;
        return rayOrigin + rayDir * t;
    }


}

