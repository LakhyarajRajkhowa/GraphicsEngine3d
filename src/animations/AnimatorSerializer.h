#pragma once

#include <external/json.hpp>
#include "animations/AnimatorController.h"

namespace Lengine
{
    using json = nlohmann::json;

    class AnimatorSerializer
    {
    public:

        static json Save(const AnimatorController& controller)
        {
            json j;

            j["currentState"] =
                controller.currentState;
            // =====================================================
            // Float Params
            // =====================================================

            json floatParams;

            for (const auto& [name, value] : controller.floatParams)
                floatParams[name] = value;

            j["floatParams"] = floatParams;

            // =====================================================
            // Bool Params
            // =====================================================

            json boolParams;

            for (const auto& [name, value] : controller.boolParams)
                boolParams[name] = value;

            j["boolParams"] = boolParams;

            // =====================================================
            // Nodes
            // =====================================================

            json jNodes = json::array();

            for (const auto& node : controller.nodes)
            {
                json jNode;

                jNode["type"] = (int)node.type;

                jNode["nodeName"] = node.nodeName;
                jNode["weight"] = node.weight;

                jNode["normalisedClipTime"] = node.normalisedClipTime;

                jNode["clipID"] = node.clipID.toUint64();
                jNode["clipTime"] = node.clipTime;
                jNode["clipDuration"] = node.clipDuration;
                jNode["looping"] = node.looping;

                jNode["parameterName"] = node.parameterName;
                jNode["parameterNameX"] = node.parameterNameX;
                jNode["parameterNameY"] = node.parameterNameY;
                jNode["playbackSpeed"] = node.playbackSpeed;

                jNode["baseNodeIndex"] = node.baseNodeIndex;
                jNode["overlayNodeIndex"] = node.overlayNodeIndex;

                jNode["boneMaskID"] = node.boneMaskID.toUint64();

                // -------------------------------------------------
                // Bone Mask
                // -------------------------------------------------

                jNode["boneMask"] = node.boneMask;

                // -------------------------------------------------
                // Blend1D Entries
                // -------------------------------------------------

                json blend1D = json::array();

                for (const auto& entry : node.blend1DEntries)
                {
                    json e;

                    e["animID"] = entry.animID.toUint64();
                    e["threshold"] = entry.threshold;
                    e["time"] = entry.time;

                    blend1D.push_back(e);
                }

                jNode["blend1DEntries"] = blend1D;

                // -------------------------------------------------
                // Blend2D Entries
                // -------------------------------------------------

                json blend2D = json::array();

                for (const auto& entry : node.blend2DEntries)
                {
                    json e;

                    e["animID"] = entry.animID.toUint64();

                    e["position"] =
                    {
                        entry.position.x,
                        entry.position.y
                    };

                    e["time"] = entry.time;

                    blend2D.push_back(e);
                }

                jNode["blend2DEntries"] = blend2D;

                jNodes.push_back(jNode);
            }

            j["nodes"] = jNodes;

            // =====================================================
            // States
            // =====================================================

            json jStates = json::array();

            for (const auto& state : controller.states)
            {
                json s;

                s["name"] = state.name;
                s["rootNodeIndex"] = state.rootNodeIndex;

                jStates.push_back(s);
            }

            j["states"] = jStates;

            // =====================================================
            // Transitions
            // =====================================================

            json jTransitions = json::array();

            for (const auto& transition : controller.transitions)
            {
                json t;

                t["fromState"] = transition.fromState;
                t["toState"] = transition.toState;
                t["duration"] = transition.duration;

                t["hasExitTime"] = transition.hasExitTime;
                t["exitTime"] = transition.exitTime;

                json conditions = json::array();

                for (const auto& cond : transition.conditions)
                {
                    json c;

                    c["paramName"] = cond.paramName;
                    c["op"] = (int)cond.op;

                    if (std::holds_alternative<float>(cond.value))
                    {
                        c["valueType"] = "float";
                        c["value"] = std::get<float>(cond.value);
                    }
                    else
                    {
                        c["valueType"] = "bool";
                        c["value"] = std::get<bool>(cond.value);
                    }

                    conditions.push_back(c);
                }

                t["conditions"] = conditions;

                jTransitions.push_back(t);
            }

            j["transitions"] = jTransitions;

            return j;
        }

        static void Load(const json& j, AnimatorController& controller)
        {
            controller = AnimatorController();

            if (j.contains("currentState")) {
                controller.currentState = j["currentState"];
            }
            // =====================================================
            // Float Params
            // =====================================================

            if (j.contains("floatParams"))
            {
                for (auto& [name, value] :
                    j["floatParams"].items())
                {
                    controller.floatParams[name] =
                        value.get<float>();
                }
            }

            // =====================================================
            // Bool Params
            // =====================================================

            if (j.contains("boolParams"))
            {
                for (auto& [name, value] :
                    j["boolParams"].items())
                {
                    controller.boolParams[name] =
                        value.get<bool>();
                }
            }

            // =====================================================
            // Nodes
            // =====================================================

            if (j.contains("nodes"))
            {
                for (const auto& jNode : j["nodes"])
                {
                    BlendNode node;

                    node.type =
                        (BlendNodeType)jNode.value("type", 0);

                    node.nodeName =
                        jNode.value("nodeName", "");

                    node.weight =
                        jNode.value("weight", 1.0f);

                    node.normalisedClipTime =
                        jNode.value("normalisedClipTime", 0.0f);

                    node.clipID =
                        UUID(jNode.value("clipID", 0ull));

                    node.clipTime =
                        jNode.value("clipTime", 0.0f);

                    node.clipDuration =
                        jNode.value("clipDuration", 1.0f);

                    node.looping =
                        jNode.value("looping", true);

                    node.parameterName =
                        jNode.value("parameterName", "");

                    node.parameterNameX =
                        jNode.value("parameterNameX", "");

                    node.parameterNameY =
                        jNode.value("parameterNameY", "");

                    node.playbackSpeed =
                        jNode.value("playbackSpeed", 1.0f);

                    node.baseNodeIndex =
                        jNode.value("baseNodeIndex", -1);

                    node.overlayNodeIndex =
                        jNode.value("overlayNodeIndex", -1);

                    node.boneMaskID =
                        UUID(jNode.value("boneMaskID", 0ull));

                    if (jNode.contains("boneMask"))
                    {
                        node.boneMask =
                            jNode["boneMask"]
                            .get<std::vector<float>>();
                    }

                    if (jNode.contains("blend1DEntries"))
                    {
                        for (const auto& e :
                            jNode["blend1DEntries"])
                        {
                            BlendNode::Blend1DEntry entry;

                            entry.animID =
                                UUID(e.value("animID", 0ull));

                            entry.threshold =
                                e.value("threshold", 0.0f);

                            entry.time =
                                e.value("time", 0.0f);

                            node.blend1DEntries.push_back(entry);
                        }
                    }

                    if (jNode.contains("blend2DEntries"))
                    {
                        for (const auto& e :
                            jNode["blend2DEntries"])
                        {
                            BlendNode::Blend2DEntry entry;

                            entry.animID =
                                UUID(e.value("animID", 0ull));

                            if (e.contains("position"))
                            {
                                entry.position.x =
                                    e["position"][0];

                                entry.position.y =
                                    e["position"][1];
                            }

                            entry.time =
                                e.value("time", 0.0f);

                            node.blend2DEntries.push_back(entry);
                        }
                    }

                    controller.nodes.push_back(node);
                }
            }

            // =====================================================
            // States
            // =====================================================

            if (j.contains("states"))
            {
                for (const auto& s : j["states"])
                {
                    AnimState state;

                    state.name =
                        s.value("name", "");

                    state.rootNodeIndex =
                        s.value("rootNodeIndex", -1);

                    controller.states.push_back(state);
                }
            }

            // =====================================================
            // Transitions
            // =====================================================

            if (j.contains("transitions"))
            {
                for (const auto& t : j["transitions"])
                {
                    AnimTransition transition;

                    transition.fromState =
                        t.value("fromState", -1);

                    transition.toState =
                        t.value("toState", -1);

                    transition.duration =
                        t.value("duration", 0.2f);

                    transition.hasExitTime =
                        t.value("hasExitTime", false);

                    transition.exitTime =
                        t.value("exitTime", 0.0f);

                    if (t.contains("conditions"))
                    {
                        for (const auto& c :
                            t["conditions"])
                        {
                            TransitionCondition cond;

                            cond.paramName =
                                c.value("paramName", "");

                            cond.op =
                                (ConditionOp)c.value("op", 0);

                            std::string type =
                                c.value("valueType", "float");

                            if (type == "float")
                            {
                                cond.value =
                                    c["value"].get<float>();
                            }
                            else
                            {
                                cond.value =
                                    c["value"].get<bool>();
                            }

                            transition.conditions.push_back(cond);
                        }
                    }

                    controller.transitions.push_back(
                        transition);
                }
            }
        }
    };
}