#pragma once

#include <vector>
#include <iostream>

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

    // TODO : Additive isn't working as expected

    inline Pose AdditivePoses(
        const Pose& base,
        const Pose& overlay,
        const Pose& reference,
        float weight,
        const std::vector<float>* mask = nullptr)
    {
        Pose result = base;

        for (size_t i = 0; i < base.BoneCount(); i++)
        {
            float w = weight * ((mask && i < mask->size()) ? (*mask)[i] : 1.0f);
            w = glm::clamp(w, 0.0f, 1.0f);

            if (w < 1e-4f) continue;

            glm::vec3 deltaPos = overlay.bones[i].position - reference.bones[i].position;
            glm::quat deltaRot = overlay.bones[i].rotation * glm::inverse(reference.bones[i].rotation);
            glm::vec3 deltaScl = overlay.bones[i].scale - reference.bones[i].scale;

            // Apply weighted rotation delta on top of base rotation
            glm::quat weightedDeltaRot = glm::slerp(glm::quat(1, 0, 0, 0), deltaRot, w);

            result.bones[i].position = base.bones[i].position + deltaPos * w;
            result.bones[i].rotation = weightedDeltaRot * base.bones[i].rotation;  
            result.bones[i].scale = base.bones[i].scale + deltaScl * w;

            float angleDeg = glm::degrees(glm::angle(deltaRot));
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

            accum = sample;
            runningWeight = weight;
            any = true;
            return;
        }

        runningWeight += weight;

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