#pragma once

#include "animations/AnimatorController.h"

namespace Lengine
{

    struct AnimationComponent
    {
        std::vector<UUID> animationIDs;
        UUID tposeAnimationID;
        Pose tPose;
        std::unordered_map<UUID, std::string> animationNames;
        std::unordered_map<std::string, UUID> animationNameToID;

        AnimatorController animator;

        std::vector<glm::mat4> finalBoneMatrices;
        std::vector<glm::mat4> globalBoneTransforms;

        AnimationComponent() = default;

        AnimationComponent(const std::vector<UUID>& animations)
            : animationIDs(animations)
        {}
    };

    inline UUID GetAnimationIDByName(
        const AnimationComponent& animComp,
        const std::string& name)
    {
        auto it = animComp.animationNameToID.find(name);

        if (it != animComp.animationNameToID.end())
            return it->second;

        return UUID::Null;
    }

}