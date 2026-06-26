#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>   // quat, quat_cast, toMat4
#include <glm/gtx/quaternion.hpp>   // eulerAngles, angleAxis helpers
#include <glm/gtc/matrix_transform.hpp>

#include "scene/components/ComponentStorage.h"

#include "utils/UUID.h"

namespace Lengine {
    class TransformSystem
    {
    public:


        static void DecomposeMatrix(
            const glm::mat4& m,
            glm::vec3& position,
            glm::quat& rotation,     
            glm::vec3& scale
        )
        {
            position = glm::vec3(m[3]);

            glm::vec3 x = glm::vec3(m[0]);
            glm::vec3 y = glm::vec3(m[1]);
            glm::vec3 z = glm::vec3(m[2]);

            scale.x = glm::length(x);
            scale.y = glm::length(y);
            scale.z = glm::length(z);

            if (scale.x != 0.0f) x /= scale.x;
            if (scale.y != 0.0f) y /= scale.y;
            if (scale.z != 0.0f) z /= scale.z;

            glm::mat3 rotMat;
            rotMat[0] = x;
            rotMat[1] = y;
            rotMat[2] = z;

            rotation = glm::quat_cast(rotMat);  
        }

        void Update(
            ComponentStorage<TransformComponent>& transforms,
            const ComponentStorage<HierarchyComponent>& hierarchys,
            const std::vector<Entity> rootEntites
        );

        void UpdateWorldTransformRecursive(
            Entity entityID,
            const glm::mat4& parentWorld,
            bool parentWorldDirty,
            ComponentStorage<TransformComponent>& transforms,
            const ComponentStorage<HierarchyComponent>& hierarchys
        );

        static void DecomposeTRS(const glm::mat4& m, glm::vec3& outPos, glm::quat& outRot, glm::vec3& outScale)
        {
            outPos = glm::vec3(m[3]);

            glm::vec3 col0(m[0]);
            glm::vec3 col1(m[1]);
            glm::vec3 col2(m[2]);

            outScale.x = glm::length(col0);
            outScale.y = glm::length(col1);
            outScale.z = glm::length(col2);

            // avoid divide-by-zero on a degenerate/zero-scale axis
            glm::mat3 rotMat(
                outScale.x > 0.0f ? col0 / outScale.x : col0,
                outScale.y > 0.0f ? col1 / outScale.y : col1,
                outScale.z > 0.0f ? col2 / outScale.z : col2
            );

            outRot = glm::quat_cast(rotMat);
        }
    private:
    };


}

