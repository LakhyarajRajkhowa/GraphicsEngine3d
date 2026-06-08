#include "AnimationSystem.h"
#include <cmath>
#include <numeric>
#include <algorithm>

#include "utils/Profiler.h"

namespace Lengine
{

    // TODO : Optimise using SoA and parallel animation

    void AnimationSystem::Update(
        ComponentStorage<AnimationComponent>& animComponents,
        ComponentStorage<SkeletonComponent>& skeletons,
        float dt
    )
    {
        auto& dense = animComponents.GetDense();
        auto& entities = animComponents.GetEntities();

        for (size_t i = 0; i < dense.size(); ++i)
        {
            AnimationComponent& anim = dense[i];
            const Entity         entity = entities[i];
            AnimatorController& ctrl = anim.animator;

            if (!ctrl.IsValid())
                continue;

            if (!skeletons.Has(entity))
                continue;

            auto& sk = skeletons.Get(entity);

            if (sk.skeletonID == UUID::Null)
                continue;

            Skeleton* skeleton = assetManager.GetSkeleton(sk.skeletonID);

            if (!skeleton)
                continue;

            if (anim.finalBoneMatrices.size() != skeleton->bones.size())
                anim.finalBoneMatrices.resize(skeleton->bones.size(), glm::mat4(1.0f));

            size_t boneCount = skeleton->bones.size();

            currentFloatParams = &ctrl.floatParams;

            ctrl.CheckTransitions();

            AnimState* curState = ctrl.GetCurrentState();
            AnimState* nextState = ctrl.GetNextState();

            if (!curState)
                continue;

            if (ctrl.isTransitioning && nextState)
            {
                ctrl.transitionProgress += dt / ctrl.transitionDuration;

                Pose poseA = EvaluateNode(curState->node, boneCount, dt);
                Pose poseB = EvaluateNode(nextState->node, boneCount, dt);

                if (ctrl.transitionProgress >= 1.0f)
                {
                    ctrl.CompleteTransition();
                    PoseToMatrices(*skeleton, poseB, anim.finalBoneMatrices);
                }
                else
                {
                    Pose blended = BlendPoses(poseA, poseB, ctrl.transitionProgress);
                    PoseToMatrices(*skeleton, blended, anim.finalBoneMatrices);
                }
            }
            else
            {
                Pose pose = EvaluateNode(curState->node, boneCount, dt);
                PoseToMatrices(*skeleton, pose, anim.finalBoneMatrices);
            }

            currentFloatParams = nullptr;
        }
    }

    Pose AnimationSystem::EvaluateNode(BlendNode& node, size_t boneCount, float dt)
    {
        switch (node.type)
        {
        case BlendNodeType::Clip:
            return EvaluateClip(node, boneCount, dt);

        case BlendNodeType::Blend1D:
            return EvaluateBlend1D(node, boneCount, dt,
                currentFloatParams ? *currentFloatParams
                : std::unordered_map<std::string, float>{});
        }

        return Pose(boneCount);
    }

    Pose AnimationSystem::EvaluateClip(BlendNode& node, size_t boneCount, float dt)
    {
        Animation* clip = assetManager.GetAnimation(node.clipID);

        if (!clip)
            return Pose(boneCount);

        node.clipTime += dt * clip->ticksPerSecond;

        if (node.looping)
            node.clipTime = fmod(node.clipTime, clip->duration);
        else
            node.clipTime = std::min(node.clipTime, clip->duration);

        return SamplePose(*clip, node.clipTime, boneCount);
    }

    Pose AnimationSystem::EvaluateBlend1D(
        BlendNode& node,
        size_t boneCount,
        float dt,
        const std::unordered_map<std::string, float>& floatParams)
    {
        auto& entries = node.blend1DEntries;

        if (entries.empty())
            return Pose(boneCount);

        float parameter = 0.0f;
        auto it = floatParams.find(node.parameterName);
        if (it != floatParams.end())
            parameter = it->second;

        // advance all clip times
        for (auto& entry : entries)
        {
            Animation* clip = assetManager.GetAnimation(entry.animID);

            if (!clip)
                continue;

            entry.time += dt * clip->ticksPerSecond * node.playbackSpeed;
            entry.time = fmod(entry.time, clip->duration);
        }

        // clamp to boundaries
        if (parameter <= entries.front().threshold)
        {
            Animation* clip = assetManager.GetAnimation(entries.front().animID);
            if (!clip) return Pose(boneCount);
            return SamplePose(*clip, entries.front().time, boneCount);
        }

        if (parameter >= entries.back().threshold)
        {
            Animation* clip = assetManager.GetAnimation(entries.back().animID);
            if (!clip) return Pose(boneCount);
            return SamplePose(*clip, entries.back().time, boneCount);
        }

        // find straddle
        size_t idxA = 0;
        size_t idxB = 1;

        for (size_t i = 0; i < entries.size() - 1; i++)
        {
            if (parameter >= entries[i].threshold && parameter < entries[i + 1].threshold)
            {
                idxA = i;
                idxB = i + 1;
                break;
            }
        }

        float threshA = entries[idxA].threshold;
        float threshB = entries[idxB].threshold;
        float t = glm::clamp((parameter - threshA) / (threshB - threshA), 0.0f, 1.0f);

        Animation* clipA = assetManager.GetAnimation(entries[idxA].animID);
        Animation* clipB = assetManager.GetAnimation(entries[idxB].animID);

        if (!clipA || !clipB)
            return Pose(boneCount);

        Pose poseA = SamplePose(*clipA, entries[idxA].time, boneCount);
        Pose poseB = SamplePose(*clipB, entries[idxB].time, boneCount);

        return BlendPoses(poseA, poseB, t);
    }

    Pose AnimationSystem::SamplePose(Animation& animation, float time, size_t boneCount)
    {
        Pose pose(boneCount);

        for (size_t i = 0; i < boneCount; i++)
        {
            int trackIndex = animation.boneTrackMap[i];

            if (trackIndex == -1)
                continue;

            auto& track = animation.tracks[trackIndex];

            pose.bones[i].position = InterpolatePosition(track, time, 1);
            pose.bones[i].rotation = InterpolateRotation(track, time, 1);
            pose.bones[i].scale = InterpolateScale(track, time, 1);
        }

        return pose;
    }

    void AnimationSystem::PoseToMatrices(
        Skeleton& skeleton,
        const Pose& pose,
        std::vector<glm::mat4>& boneMatrices)
    {
        std::vector<glm::mat4> globalTransforms(skeleton.bones.size());

        for (size_t i = 0; i < skeleton.bones.size(); i++)
        {
            const BoneTransform& bt = pose.bones[i];

            glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), bt.position);
            localTransform *= glm::toMat4(bt.rotation);
            localTransform = glm::scale(localTransform, bt.scale);

            int parent = skeleton.bones[i].parentIndex;

            if (parent == -1)
                globalTransforms[i] = localTransform;
            else
                globalTransforms[i] = globalTransforms[parent] * localTransform;

            boneMatrices[i] = globalTransforms[i] * skeleton.bones[i].inverseBindMatrix;
        }
    }

    glm::vec3 AnimationSystem::InterpolatePosition(AnimationTrack& track, float time, int delta)
    {
        if (track.positions.size() == 1)
            return track.positions[0].position;

        for (size_t i = 0; i < track.positions.size() - 1; i += delta)
        {
            if (time < track.positions[i + 1].timeStamp)
            {
                float t1 = track.positions[i].timeStamp;
                float t2 = track.positions[i + 1].timeStamp;
                float factor = (time - t1) / (t2 - t1);

                return glm::mix(
                    track.positions[i].position,
                    track.positions[i + 1].position,
                    factor);
            }
        }

        return track.positions.back().position;
    }

    glm::quat AnimationSystem::InterpolateRotation(AnimationTrack& track, float time, int delta)
    {
        if (track.rotations.size() == 1)
            return track.rotations[0].rotation;

        for (size_t i = 0; i < track.rotations.size() - 1; i += delta)
        {
            if (time < track.rotations[i + 1].timeStamp)
            {
                float t1 = track.rotations[i].timeStamp;
                float t2 = track.rotations[i + 1].timeStamp;
                float factor = (time - t1) / (t2 - t1);

                return glm::slerp(
                    track.rotations[i].rotation,
                    track.rotations[i + 1].rotation,
                    factor);
            }
        }

        return track.rotations.back().rotation;
    }

    glm::vec3 AnimationSystem::InterpolateScale(AnimationTrack& track, float time, int delta)
    {
        if (track.scales.size() == 1)
            return track.scales[0].scale;

        for (size_t i = 0; i < track.scales.size() - 1; i += delta)
        {
            if (time < track.scales[i + 1].timeStamp)
            {
                float t1 = track.scales[i].timeStamp;
                float t2 = track.scales[i + 1].timeStamp;
                float factor = (time - t1) / (t2 - t1);

                return glm::mix(
                    track.scales[i].scale,
                    track.scales[i + 1].scale,
                    factor);
            }
        }

        return track.scales.back().scale;
    }

}