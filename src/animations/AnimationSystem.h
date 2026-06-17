#pragma once

#include "scene/components/ComponentStorage.h"
#include "resources/AssetManager.h"
#include "animations/Pose.h"

namespace Lengine
{

    class AnimationSystem
    {
    public:
        AnimationSystem(AssetManager& assetManager) : assetManager(assetManager) {}

        void Update(
            ComponentStorage<AnimationComponent>& animations,
            ComponentStorage<SkeletonComponent>& skeletons,
            float dt
        );

        Pose SamplePose(Animation& animation, float time, size_t boneCount);

        void PoseToMatrices(
            Skeleton& skeleton,
            const Pose& pose,
            std::vector<glm::mat4>& boneMatrices,
            std::vector<glm::mat4>& globalTransforms); 

    private:
        AssetManager& assetManager;

        Pose EvaluateNode(AnimatorController& ctrl, int nodeIndex, size_t boneCount, float dt);
        Pose EvaluateClip(BlendNode& node, size_t boneCount, float dt);
        Pose EvaluateBlend1D(BlendNode& node, size_t boneCount, float dt,
            const std::unordered_map<std::string, float>& floatParams);
        Pose EvaluateMasked(AnimatorController& ctrl, BlendNode& node, size_t boneCount, float dt);

        glm::vec3 InterpolatePosition(AnimationTrack& track, float time, int delta);
        glm::quat InterpolateRotation(AnimationTrack& track, float time, int delta);
        glm::vec3 InterpolateScale(AnimationTrack& track, float time, int delta);

        // stored per-Update so EvaluateNode can read parameters without threading them through
        const std::unordered_map<std::string, float>* currentFloatParams = nullptr;



    };

   

}