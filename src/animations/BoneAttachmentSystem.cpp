#include "BoneAttachmentSystem.h"
#include "transform/TransformSystem.h"

namespace Lengine
{

    void BoneAttachmentSystem::Init(Scene& scene) {
        auto& registry = scene.GetRegistry();

        registry.boneAttachments.onAdd = [this, scenePtr = &scene](Entity e, BoneAttachmentComponent& bn)
            {
                bn.modelRoot = scenePtr->GetRootParent(e);
                bn.skinnedRoot = FindRootSkinnedEntity(scenePtr->GetRegistry().meshFilters, bn.modelRoot);
            };
    }

    void BoneAttachmentSystem::Update(
        ComponentStorage<BoneAttachmentComponent>& attachments,
        ComponentStorage<AnimationComponent>& animations,
        ComponentStorage<SkeletonComponent>& skeletons,
        ComponentStorage<TransformComponent>& transforms)
    {
        auto& dense = attachments.GetDense();
        auto& entities = attachments.GetEntities();

        for (size_t i = 0; i < dense.size(); i++)
        {
            const BoneAttachmentComponent& attachment = dense[i];
            const Entity                   entity = entities[i];
  

            Entity root = attachment.modelRoot;
            Entity skinnedRoot = attachment.skinnedRoot;

            if (root == NullEntity || skinnedRoot == NullEntity) return;

            if (!animations.Has(root) || !skeletons.Has(root) || !transforms.Has(root))
                continue;

            const AnimationComponent& anim = animations.Get(root);

            if (attachment.boneIndex >= (int)anim.globalBoneTransforms.size())
                continue;

            const SkeletonComponent& sk = skeletons.Get(root);

            if (sk.skeletonID == UUID::Null)
                continue;

            Skeleton* skeleton = assetManager.GetSkeleton(sk.skeletonID);

            if (!skeleton || attachment.boneIndex >= (int)skeleton->bones.size())
                continue;



            const glm::mat4& globalBoneTransform = anim.globalBoneTransforms[attachment.boneIndex];
            const glm::mat4& rootWorld = transforms.Get(skinnedRoot).worldMatrix;

            glm::mat4 finalWorld = rootWorld * globalBoneTransform * attachment.offset;


            if (!transforms.Has(entity))
                continue;

            TransformComponent& t = transforms.Get(entity);

            t.worldMatrix = finalWorld;
            t.worldDirty = false;

        }
    }

}