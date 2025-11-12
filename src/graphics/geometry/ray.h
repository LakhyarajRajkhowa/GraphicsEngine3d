#pragma once
#include <glm/glm.hpp>
#include <iostream>
namespace Lengine {
    inline glm::vec3 ComputeRayDirection(
        const float& mouseX,
        const float& mouseY,
        const float& screenWidth,
        const float& screenHeight,
        const glm::mat4& view,
        const glm::mat4& projection
    )
    {
        glm::vec2 vpSize = { screenWidth, screenHeight };
     
        float vx = mouseX;
        float vy = mouseY;
        // Mouse outside viewport? return zero ray
        if (vx < 0 || vy < 0 || vx > vpSize.x || vy > vpSize.y)
            return glm::vec3(0);

        // Convert to NDC inside viewport
        float ndcX = 2.0f * (vx / vpSize.x) - 1.0f;
        float ndcY = -2.0f * (vy / vpSize.y) + 1.0f;

        glm::vec4 rayClip = { ndcX, ndcY, -1.0f, 1.0f };

        // Clip → Eye
        glm::vec4 rayEye = glm::inverse(projection) * rayClip;
        rayEye = { rayEye.x, rayEye.y, -1.0f, 0.0f };

        // Eye → World
        glm::vec3 rayWorld =
            glm::normalize(glm::vec3(glm::inverse(view) * rayEye));

        return rayWorld;


    }

    inline bool rayIntersectsSphere(
        const glm::vec3& rayOrigin,
        const glm::vec3& rayDir,
        const glm::vec3& center,
        float radius
    ) {
        

        glm::vec3 oc = rayOrigin - center;
        float a = glm::dot(rayDir, rayDir);
        float b = 2.0f * glm::dot(oc, rayDir);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;
        bool hit = (discriminant >= 0.0f);
        return hit;
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

