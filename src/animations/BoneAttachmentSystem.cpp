#include "BoneAttachmentSystem.h"
#include "transform/TransformSystem.h"

namespace Lengine
{

    void BoneAttachmentSystem::Init(Scene& scene) {
        auto& registry = scene.GetRegistry();

        registry.boneAttachments.onAdd = [this, scenePtr = &scene](Entity e, BoneAttachmentComponent& bn)
            {
                bn.modelRoot = bn.modelRoot == NullEntity ? scenePtr->GetRootParent(e) : bn.modelRoot;
                bn.skinnedRoot = bn.skinnedRoot == NullEntity ? FindRootSkinnedEntity(scenePtr->GetRegistry().meshFilters, bn.modelRoot) : bn.skinnedRoot;
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

            if (!skeleton || attachment.boneIndex >= (int)skeleton->bones.size() || attachment.boneIndex < 0)
                continue;

            const glm::mat4& globalBoneTransform = anim.globalBoneTransforms[attachment.boneIndex];
            const glm::mat4& rootWorld = transforms.Get(skinnedRoot).worldMatrix;

            glm::mat4 finalWorld = rootWorld * globalBoneTransform * attachment.offset.matrix;


            if (!transforms.Has(entity))
                continue;

            TransformComponent& t = transforms.Get(entity);

            // NOTE : It is expected that the colliders are added individually 
            // without any hierarchies , therefore the transforms are updated immediately
            t.worldMatrix = finalWorld;
            t.worldDirty = false;

        }
    }

    bool BoneAttachmentSystem::TryGetBoneAttachedWorldMatrix(
        const Entity& entity,
        ComponentStorage<BoneAttachmentComponent>& attachments,
        ComponentStorage<AnimationComponent>& animations,
        ComponentStorage<SkeletonComponent>& skeletons,
        ComponentStorage<TransformComponent>& transforms,
        glm::mat4& outWorld)
    {
        if (!attachments.Has(entity))
        {
            return false;
        }

        const BoneAttachmentComponent& attachment = attachments.Get(entity);

        Entity root = attachment.modelRoot;
        Entity skinnedRoot = attachment.skinnedRoot;

        if (root == NullEntity || skinnedRoot == NullEntity)
        {
            std::cerr << "[BoneAttachment] Entity " << entity
                << " has unresolved modelRoot/skinnedRoot (root="
                << root << ", skinnedRoot=" << skinnedRoot << ")\n";
            return false;
        }

        if (!animations.Has(root) || !skeletons.Has(root) || !transforms.Has(skinnedRoot))
        {
            std::cerr << "[BoneAttachment] Entity " << entity
                << " missing required component on root/skinnedRoot - "
                << "animations.Has(root)=" << animations.Has(root)
                << ", skeletons.Has(root)=" << skeletons.Has(root)
                << ", transforms.Has(skinnedRoot)=" << transforms.Has(skinnedRoot) << "\n";
            return false;
        }

        const AnimationComponent& anim = animations.Get(root);

        if (attachment.boneIndex < 0 ||
            attachment.boneIndex >= (int)anim.globalBoneTransforms.size())
        {
            std::cerr << "[BoneAttachment] Entity " << entity
                << " boneIndex " << attachment.boneIndex
                << " out of range (globalBoneTransforms.size()="
                << anim.globalBoneTransforms.size() << ")\n";
            return false;
        }

        const SkeletonComponent& sk = skeletons.Get(root);

        if (sk.skeletonID == UUID::Null)
        {
            std::cerr << "[BoneAttachment] Entity " << entity
                << " root entity " << root << " has null skeletonID\n";
            return false;
        }

        Skeleton* skeleton = assetManager.GetSkeleton(sk.skeletonID);

        if (!skeleton || attachment.boneIndex >= (int)skeleton->bones.size())
        {
            std::cerr << "[BoneAttachment] Entity " << entity
                << " skeleton lookup failed or boneIndex " << attachment.boneIndex
                << " out of range (skeleton="
                << (skeleton ? "valid" : "null")
                << ", bones.size()=" << (skeleton ? skeleton->bones.size() : 0) << ")\n";
            return false;
        }

        const glm::mat4& globalBoneTransform = anim.globalBoneTransforms[attachment.boneIndex];
        const glm::mat4& rootWorld = transforms.Get(skinnedRoot).worldMatrix;

        outWorld = rootWorld * globalBoneTransform * attachment.offset.matrix;
        return true;
    }

   
}