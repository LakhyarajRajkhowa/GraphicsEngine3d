#pragma once

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Lengine
{

    struct BoneTransform
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f);
    };

    struct Pose
    {
        std::vector<BoneTransform> bones;

        Pose() = default;

        explicit Pose(size_t boneCount)
            : bones(boneCount)
        {}

        void Resize(size_t boneCount)
        {
            bones.resize(boneCount);
        }

        size_t BoneCount() const
        {
            return bones.size();
        }
    };

    inline Pose BlendPoses(
        const Pose& a,
        const Pose& b,
        float t,
        const std::vector<float>* mask = nullptr)
    {
        Pose result(a.BoneCount());

        for (size_t i = 0; i < a.BoneCount(); i++)
        {
            float w = (mask && i < mask->size()) ? (*mask)[i] : t;
            w = glm::clamp(w, 0.0f, 1.0f);

            result.bones[i].position = glm::mix(a.bones[i].position, b.bones[i].position, w);
            result.bones[i].rotation = glm::slerp(a.bones[i].rotation, b.bones[i].rotation, w);
            result.bones[i].scale = glm::mix(a.bones[i].scale, b.bones[i].scale, w);
        }

        return result;
    }

}