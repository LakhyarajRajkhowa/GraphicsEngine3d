#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

#include "animations/Pose.h"

#include "utils/UUID.h"

namespace Lengine
{

    // ─── Blend Node ──────────────────────────────────────────────────────────

    enum class BlendNodeType { Clip, Blend1D };

    struct BlendNode
    {
        BlendNodeType type = BlendNodeType::Clip;

        // Clip
        UUID  clipID = UUID::Null;
        float clipTime = 0.0f;
        bool  looping = true;

        // Blend1D
        struct Blend1DEntry
        {
            UUID  animID = UUID::Null;
            float threshold = 0.0f;
            float time = 0.0f;
        };

        std::vector<Blend1DEntry> blend1DEntries;
        std::string               parameterName;
        float                     playbackSpeed = 1.0f;

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

        void AddEntry(UUID animID, float threshold)
        {

            blend1DEntries.push_back({ animID, threshold, 0.0f });

            std::sort(blend1DEntries.begin(), blend1DEntries.end(),
                [](const Blend1DEntry& a, const Blend1DEntry& b) {
                    return a.threshold < b.threshold;
                });
        }
    };


    // ─── Transition Condition ────────────────────────────────────────────────

    enum class ConditionOp { Greater, Less, Equal, NotEqual };

    struct TransitionCondition
    {
        std::string  paramName;
        ConditionOp  op;
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


    // ─── State ───────────────────────────────────────────────────────────────

    struct AnimState
    {
        std::string name;
        BlendNode   node;
    };


    // ─── Transition ──────────────────────────────────────────────────────────

    struct AnimTransition
    {
        int                             fromState = -1;  // -1 = any state
        int                             toState = -1;
        float                           duration = 0.2f;
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


    // ─── Animator Controller ─────────────────────────────────────────────────

    struct AnimatorController
    {
        std::vector<AnimState>      states;
        std::vector<AnimTransition> transitions;

        std::unordered_map<std::string, float> floatParams;
        std::unordered_map<std::string, bool>  boolParams;

        int   currentState = -1;
        int   nextState = -1;
        float transitionProgress = 0.0f;
        float transitionDuration = 0.0f;
        bool  isTransitioning = false;
        float nextStateTime = 0.0f;

        // ── Setup API ────────────────────────────────────────────────────────

        int AddState(const std::string& name, BlendNode node)
        {
            int idx = (int)states.size();
            states.push_back({ name, std::move(node) });
            return idx;
        }

        void AddTransition(
            int fromState,
            int toState,
            float duration,
            std::vector<TransitionCondition> conditions = {})
        {
            transitions.push_back({ fromState, toState, duration, std::move(conditions) });
        }

        void AddTransitionFromAny(
            int toState,
            float duration,
            std::vector<TransitionCondition> conditions = {})
        {
            transitions.push_back({ -1, toState, duration, std::move(conditions) });
        }

        void SetDefaultState(int stateIndex)
        {
            currentState = stateIndex;
        }

        // ── Parameter API ────────────────────────────────────────────────────

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

        // ── Transition Check (called by AnimationSystem) ──────────────────────

        bool CheckTransitions()
        {
            if (isTransitioning)
                return false;

            for (const auto& t : transitions)
            {
                if (t.CanFire(currentState, floatParams, boolParams))
                {
                    nextState = t.toState;
                    transitionDuration = t.duration;
                    transitionProgress = 0.0f;
                    nextStateTime = 0.0f;
                    isTransitioning = true;
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