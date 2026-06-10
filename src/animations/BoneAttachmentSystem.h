#pragma once

#include "scene/components/ComponentStorage.h"
#include "scene/components/BoneAttachmentComponent.h"
#include "scene/components/Transform.h"
#include "scene/components/AnimatonComponent.h"
#include "scene/components/SkeletonComponent.h"
#include "resources/AssetManager.h"

namespace Lengine
{

    class BoneAttachmentSystem
    {
    public:
        BoneAttachmentSystem(AssetManager& assetManager)
            : assetManager(assetManager) {}

        void Init(Scene& scnee);


        void Update(
            ComponentStorage<BoneAttachmentComponent>& attachments,
            ComponentStorage<AnimationComponent>& animations,
            ComponentStorage<SkeletonComponent>& skeletons,
            ComponentStorage<TransformComponent>& transforms
        );


    private:
        AssetManager& assetManager;

        // this finds the root skinned mesh (can be the entity with mesh itself) 
        Entity FindRootSkinnedEntity(ComponentStorage<MeshFilter>& meshFilters, Entity rootEntity)
        {
            const auto& mfDense = meshFilters.GetDense();
            const auto& mfEntities = meshFilters.GetEntities();

            for (size_t i = 0; i < mfDense.size(); ++i)
            {
                if (mfDense[i].rootParent == rootEntity)
                {
                    std::cout << "found skinned Root\n";
                    return mfEntities[i];
                }
            }
            std::cout << "not found skinned Root\n";
            return NullEntity;
        }
    };

   
}