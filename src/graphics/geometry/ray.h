#pragma once
#include <glm/glm.hpp>
#include <iostream>
namespace Lengine {
    inline glm::vec3 ComputeRayDirection(
        float mouseX,
        float mouseY,
        float screenWidth,
        float screenHeight,
        const glm::mat4& view,
        const glm::mat4& projection)
    {
      
        float x = (2.0f * mouseX) / screenWidth - 1.0f;
        float y = 1.0f - (2.0f * mouseY) / screenHeight;
        glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
        glm::vec4 rayEye = glm::inverse(projection) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
        glm::vec3 rayDir = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));

        return rayDir;
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

