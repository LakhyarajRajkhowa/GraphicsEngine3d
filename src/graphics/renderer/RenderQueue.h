#pragma once

#include <vector>
#include <algorithm>
#include <cstdint>
#include <glm/glm.hpp>

#include "graphics/geometry/Mesh.h"
#include "graphics/material/Material.h"
#include "scene/Entity.h"
#include "utils/UUID.h"

namespace Lengine {


    struct RenderItem {
        Mesh* mesh = nullptr;
        ResolvedMaterial  material;          
        glm::mat4         modelMatrix = glm::mat4(1.0f);
        Entity            entity = 0;

        bool              hasSkeleton = false;
        const std::vector<glm::mat4>* boneMatrices = nullptr;
        const std::vector<int>* bonePalette = nullptr;

        uint64_t sortKey = 0;
    };

    inline uint64_t BuildSortKey(
        float    depthFromCamera,
        uint32_t shaderID,
        uint32_t materialUUIDbits,
        bool     isTransparent = false)
    {

        uint32_t depthBits = static_cast<uint32_t>(depthFromCamera * 1000.0f);
        if (isTransparent)
            depthBits = ~depthBits; 

        uint64_t layer = isTransparent ? 1ULL : 0ULL;
        uint64_t shader = static_cast<uint64_t>(shaderID) & 0xFFFFULL;
        uint64_t depth = static_cast<uint64_t>(depthBits) & 0xFFFFFFFFULL;
        uint64_t matBits = static_cast<uint64_t>(materialUUIDbits) & 0xFFFULL;

        return (layer << 60)
            | (shader << 44)
            | (depth << 12)
            | (matBits << 0);
    }

    inline uint64_t BuildDepthSortKey(float depthFromCamera, bool isTransparent = false)
    {
        uint32_t depthBits = static_cast<uint32_t>(depthFromCamera * 1000.0f);
        if (isTransparent)
            depthBits = ~depthBits; // flip for back-to-front

        return static_cast<uint64_t>(depthBits);
    }


    class RenderQueue {
    public:

        // reserve count depends on #RenderItems = #MeshRenderers
        explicit RenderQueue(size_t reserveCount = 512)
        {
            items.reserve(reserveCount);
        }

        void Clear()
        {
            items.clear();
        }

        void Submit(RenderItem item)
        {
            items.push_back(std::move(item));
        }

        // Sort by sort key ascending (smaller depth -> larger depth) 
        void Sort()
        {
            std::sort(items.begin(), items.end(),
                [](const RenderItem& a, const RenderItem& b) {
                    return a.sortKey < b.sortKey;
                });
        }

        const std::vector<RenderItem>& GetItems() const { return items; }
        size_t Size() const { return items.size(); }

    private:
        std::vector<RenderItem> items;
    };

} // namespace Lengine