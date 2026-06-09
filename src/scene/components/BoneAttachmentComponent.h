#pragma once

#include <glm/glm.hpp>
#include "scene/Entity.h"

namespace Lengine
{

    struct BoneAttachmentComponent
    {
        Entity    modelRoot = NullEntity;
        Entity    skinnedRoot = NullEntity;
        int       boneIndex = -1;
        glm::mat4 offset = glm::mat4(1.0f);
    };

}