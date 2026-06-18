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
            float w = t;
            w *= (mask && i < mask->size()) ? (*mask)[i] : 1.0f;
            w = glm::clamp(w, 0.0f, 1.0f);

            result.bones[i].position = glm::mix(a.bones[i].position, b.bones[i].position, w);
            result.bones[i].rotation = glm::slerp(a.bones[i].rotation, b.bones[i].rotation, w);
            result.bones[i].scale = glm::mix(a.bones[i].scale, b.bones[i].scale, w);
        }

        return result;
    }

    inline void AccumulateWeightedPose(
        Pose& accum,
        const Pose& sample,
        float weight,
        float& runningWeight,
        bool& any)
    {
        if (!any)
        {
            // First contributing sample -- just take it directly,
            // weighted by its own share (matters if it's not the only one).
            accum = sample;
            runningWeight = weight;
            any = true;
            return;
        }

        runningWeight += weight;

        // Re-normalized blend factor: blending `accum` (which already
        // represents the weighted average of everything accumulated so
        // far) toward `sample` by t = weight / runningWeight produces the
        // correct overall weighted average, equivalent to doing it in one
        // pass -- this is the same incremental-average trick used for
        // online mean computation, applied per-bone.
        float t = weight / runningWeight;

        size_t boneCount = accum.bones.size();

        for (size_t i = 0; i < boneCount; i++)
        {
            accum.bones[i].position = glm::mix(accum.bones[i].position, sample.bones[i].position, t);
            accum.bones[i].rotation = glm::slerp(accum.bones[i].rotation, sample.bones[i].rotation, t);
            accum.bones[i].scale = glm::mix(accum.bones[i].scale, sample.bones[i].scale, t);
        }
    }


}