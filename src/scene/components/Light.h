#pragma once

#include "utils/UUID.h"

#include <glm/glm.hpp>

#define MAX_LIGHTS 64

namespace Lengine {

    enum class LightType {
        Directional = 0,
        Point = 1,
        Spotlight = 2,
        count = 3
    };

    class Light {
    public:
        Light() = default;

        Light(Entity entityID)
            : id(entityID) {}

        Light(Entity entityID,
            LightType lightType,
            const glm::vec3& lightColor,
            float lightIntensity = 1.0f)
            : id(entityID),
            type(lightType),
            color(lightColor),
            intensity(lightIntensity)
        {}

        Entity id = NullEntity;
        LightType type = LightType::Directional;

        glm::vec3 color = glm::vec3(0.10f);
        float intensity = 1.0f;

        // Point + Spot
        float range = 10.0f;

        // Spot only (degrees, engine-side)
        float innerAngle = 12.5f;
        float outerAngle = 17.5f;

        bool castShadow = false;
    };

}