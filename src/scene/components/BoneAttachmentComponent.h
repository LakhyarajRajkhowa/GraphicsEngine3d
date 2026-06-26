#pragma once

#include <glm/glm.hpp>
#include "scene/Entity.h"

namespace Lengine
{
    struct OffsetTransform
    {
        OffsetTransform()
        {
            RecalculateMatrix();
        }


        glm::vec3 position{ 0.0f };
        glm::quat rotation{ 1,0,0,0 };
        glm::vec3 scale{ 1.0f };

        glm::mat4 matrix;

        void RecalculateMatrix()
        {
            matrix =
                glm::translate(glm::mat4(1.0f), position) *
                glm::mat4_cast(rotation) *
                glm::scale(glm::mat4(1.0f), scale);
        }
    };

    struct BoneAttachmentComponent
    {
        Entity    modelRoot = NullEntity;
        Entity    skinnedRoot = NullEntity;
        int       boneIndex = -1;
        std::string boneName;
        OffsetTransform offset;
    };

}