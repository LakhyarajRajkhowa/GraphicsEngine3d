#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <algorithm>
#include <numeric>

#include <glm/glm.hpp>

#include "animations/Pose.h"
#include "utils/UUID.h"

// TODO : Animation Controller should be a reusable asset ,
//        but with instance just like Material

namespace Lengine
{

    enum class BlendNodeType { Clip, Blend1D, Blend2D, Masked };

    struct BlendNode
    {
        // Common

        BlendNodeType type = BlendNodeType::Clip;

        std::string nodeName;

        float weight = 1.0f;

        // Runtime

        float normalisedClipTime = 0.0f;

        // --- Clip Node ---

        UUID  clipID = UUID::Null;

        float clipTime = 0.0f;
        float clipDuration = 1.0f;

        bool looping = true;

        // --- Blend1D Node ---

        struct Blend1DEntry
        {
            UUID  animID = UUID::Null;

            float threshold = 0.0f;
            float time = 0.0f;
        };

        std::vector<Blend1DEntry> blend1DEntries;

        std::string parameterName;

        float playbackSpeed = 1.0f;

        // --- Blend2D Node ---

        struct Blend2DEntry
        {
            UUID  animID = UUID::Null;

            glm::vec2 position = glm::vec2(0.0f); // (paramX, paramY) sample point
            float     time = 0.0f;
        };

        std::vector<Blend2DEntry> blend2DEntries;

        std::string parameterNameX;
        std::string parameterNameY;

        // --- Masked Node ---

        int baseNodeIndex = -1;
        int overlayNodeIndex = -1;

        UUID boneMaskID = UUID::Null;

        std::vector<float> boneMask;

        // Utility

        void AddEntry(UUID animID, float threshold)
        {
            blend1DEntries.push_back({
                animID,
                threshold,
                0.0f
                });

            std::sort(
                blend1DEntries.begin(),
                blend1DEntries.end(),
                [](const Blend1DEntry& a, const Blend1DEntry& b)
                {
                    return a.threshold < b.threshold;
                });
        }

        void AddEntry2D(UUID animID, glm::vec2 position)
        {
            blend2DEntries.push_back({
                animID,
                position,
                0.0f
                });

        }

        bool CheckForExitTime(float exitTime)
        {
            normalisedClipTime =
                std::min(clipTime / clipDuration, 1.0f);

            return normalisedClipTime >= exitTime;
        }

        // Factory Functions

        static BlendNode MakeClip(UUID clipID, bool looping = true)
        {
            BlendNode n;
            n.type = BlendNodeType::Clip;
            n.clipID = clipID;
            n.looping = looping;
            return n;
        }

        static BlendNode MakeBlend1D(const std::string& paramName, float playbackSpeed = 1.0f)
        {
            BlendNode n;
            n.type = BlendNodeType::Blend1D;
            n.parameterName = paramName;
            n.playbackSpeed = playbackSpeed;
            return n;
        }

        static BlendNode MakeBlend2D(const std::string& paramNameX, const std::string& paramNameY, float playbackSpeed = 1.0f)
        {
            BlendNode n;
            n.type = BlendNodeType::Blend2D;
            n.parameterNameX = paramNameX;
            n.parameterNameY = paramNameY;
            n.playbackSpeed = playbackSpeed;
            return n;
        }

        static BlendNode MakeMasked(int baseNodeIndex, int overlayNodeIndex, std::vector<float> boneMask)
        {
            BlendNode n;
            n.type = BlendNodeType::Masked;
            n.baseNodeIndex = baseNodeIndex;
            n.overlayNodeIndex = overlayNodeIndex;
            n.boneMask = std::move(boneMask);
            return n;
        }
    };



    enum class ConditionOp { Greater, Less, Equal, NotEqual };

    struct TransitionCondition
    {
        std::string               paramName;
        ConditionOp               op;
        std::variant<float, bool> value;

        bool Evaluate(float f) const
        {
            if (std::holds_alternative<float>(value))
            {
                float v = std::get<float>(value);
                switch (op)
                {
                case ConditionOp::Greater:  return f > v;
                case ConditionOp::Less:     return f < v;
                case ConditionOp::Equal:    return f == v;
                case ConditionOp::NotEqual: return f != v;
                }
            }
            return false;
        }

        bool Evaluate(bool b) const
        {
            if (std::holds_alternative<bool>(value))
            {
                bool v = std::get<bool>(value);
                switch (op)
                {
                case ConditionOp::Equal:    return b == v;
                case ConditionOp::NotEqual: return b != v;
                default:                    return false;
                }
            }
            return false;
        }
    };



    struct AnimState
    {
        std::string name;
        int         rootNodeIndex = -1;
    };



    struct AnimTransition
    {
        int                              fromState = -1;
        int                              toState = -1;
        float                            duration = 0.2f;
        bool                             hasExitTime = false;
        float                            exitTime = 1.0f;
        std::vector<TransitionCondition> conditions;


        bool CanFire(
            int currentState,
            const std::unordered_map<std::string, float>& floats,
            const std::unordered_map<std::string, bool>& bools) const
        {
            if (fromState != -1 && fromState != currentState)
                return false;

            if (toState == currentState)
                return false;

            for (const auto& cond : conditions)
            {
                auto fit = floats.find(cond.paramName);
                if (fit != floats.end())
                {
                    if (!cond.Evaluate(fit->second))
                        return false;
                    continue;
                }

                auto bit = bools.find(cond.paramName);
                if (bit != bools.end())
                {
                    if (!cond.Evaluate(bit->second))
                        return false;
                    continue;
                }

                return false;
            }

            return true;
        }
    };



    struct AnimatorController
    {
        std::vector<BlendNode>    nodes;
        std::vector<AnimState>    states;
        std::vector<AnimTransition> transitions;

        std::unordered_map<std::string, float> floatParams;
        std::unordered_map<std::string, bool>  boolParams;

        int   currentState = -1;
        int   nextState = -1;
        float transitionProgress = 0.0f;
        float transitionDuration = 0.0f;
        bool  isTransitioning = false;
        float nextStateTime = 0.0f;


        int AddNode(BlendNode node)
        {
            int idx = (int)nodes.size();

            node.nodeName = "node_" + std::to_string(idx);

            nodes.push_back(std::move(node));
            return idx;
        }


        int AddState(const std::string& name, int rootNodeIndex)
        {
            int idx = (int)states.size();
            states.push_back({ name, rootNodeIndex });
            return idx;
        }

        int AddStateClip(const std::string& name, UUID clipID, bool looping = true)
        {
            int nodeIdx = AddNode(BlendNode::MakeClip(clipID, looping));
            return AddState(name, nodeIdx);
        }


        void AddTransition(
            int fromState,
            int toState,
            float duration,
            bool hasExitTime = false,
            float exitTime = 0.0f,
            std::vector<TransitionCondition> conditions = {})
        {
            transitions.push_back({ fromState, toState, duration, hasExitTime, exitTime,  std::move(conditions) });
        }

        void AddTransitionFromAny(
            int toState,
            float duration,
            bool hasExitTime = false,
            float exitTime = 0.0f,
            std::vector<TransitionCondition> conditions = {})
        {
            transitions.push_back({ -1, toState, duration, hasExitTime, exitTime, std::move(conditions) });
        }

        void SetDefaultState(int stateIndex)
        {
            currentState = stateIndex;
        }


        void SetFloat(const std::string& name, float value) { floatParams[name] = value; }
        void SetBool(const std::string& name, bool  value) { boolParams[name] = value; }

        float GetFloat(const std::string& name) const
        {
            auto it = floatParams.find(name);
            return it != floatParams.end() ? it->second : 0.0f;
        }

        bool GetBool(const std::string& name) const
        {
            auto it = boolParams.find(name);
            return it != boolParams.end() ? it->second : false;
        }


        bool CheckTransitions()
        {
            if (isTransitioning)
                return false;

            for (const auto& t : transitions)
            {
                if (t.hasExitTime)
                {
                    if (!nodes[states[currentState].rootNodeIndex].CheckForExitTime(t.exitTime))
                        continue;
                }

                if (t.CanFire(currentState, floatParams, boolParams))
                {
                    nextState = t.toState;
                    transitionDuration = t.duration;
                    transitionProgress = 0.0f;
                    isTransitioning = true;

                    // Reset the incoming state's node here, 
                    // so it plays from 0 during the crossfade
                    int nodeIdx = states[nextState].rootNodeIndex;
                    if (nodeIdx >= 0 && nodeIdx < (int)nodes.size())
                    {
                        BlendNode& node = nodes[nodeIdx];
                        node.clipTime = 0.0f;
                        node.normalisedClipTime = 0.0f;
                        for (auto& entry : node.blend1DEntries)
                            entry.time = 0.0f;
                        for (auto& entry : node.blend2DEntries)
                            entry.time = 0.0f;
                    }

                    nextStateTime = 0.0f;
                    return true;
                }
            }

            return false;
        }

        void CompleteTransition()
        {

            currentState = nextState;
            nextState = -1;
            transitionProgress = 0.0f;
            transitionDuration = 0.0f;
            isTransitioning = false;
            nextStateTime = 0.0f;
        }

        bool IsValid() const { return currentState >= 0 && currentState < (int)states.size(); }

        AnimState* GetCurrentState()
        {
            if (!IsValid()) return nullptr;
            return &states[currentState];
        }

        AnimState* GetNextState()
        {
            if (nextState < 0 || nextState >= (int)states.size()) return nullptr;
            return &states[nextState];
        }
    };

}