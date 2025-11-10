#pragma once
#include <glm/glm.hpp>
#include <iostream>
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
        std::cout << "---- Ray–Sphere Debug ----\n";
        std::cout << "Ray Origin: (" << rayOrigin.x << ", " << rayOrigin.y << ", " << rayOrigin.z << ")\n";
        std::cout << "Ray Dir:    (" << rayDir.x << ", " << rayDir.y << ", " << rayDir.z << ")\n";
        std::cout << "Sphere Center: (" << center.x << ", " << center.y << ", " << center.z << ")\n";
        std::cout << "Sphere Radius: " << radius << "\n";

        glm::vec3 oc = rayOrigin - center;
        std::cout << "OC Vector: (" << oc.x << ", " << oc.y << ", " << oc.z << ")\n";

        float a = glm::dot(rayDir, rayDir);
        float b = 2.0f * glm::dot(oc, rayDir);
        float c = glm::dot(oc, oc) - radius * radius;

        std::cout << "a: " << a << "\n";
        std::cout << "b: " << b << "\n";
        std::cout << "c: " << c << "\n";

        float discriminant = b * b - 4.0f * a * c;
        std::cout << "Discriminant: " << discriminant << "\n";

        bool hit = (discriminant >= 0.0f);
        std::cout << "Hit: " << (hit ? "YES" : "NO") << "\n";

        std::cout << "--------------------------\n";

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

