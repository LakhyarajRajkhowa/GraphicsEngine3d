#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>

#include <glm/glm.hpp>

namespace Lengine {


    struct SkeletonBone
    {
        std::string name;
        int parentIndex;
        glm::mat4 inverseBindMatrix;
        glm::mat4 bindMatrix;
    };

    struct SkeletonBoneNode
    {
        std::string name;
        int index = -1;
        SkeletonBoneNode* parentNode = nullptr;
        std::vector<SkeletonBoneNode*> childNodes;
    };

    class Skeleton
    {
    public:

        Skeleton() = default;


        // metadata
        UUID skeletonID;
        std::string name;
        std::string sourcePath;

        // bone data (for runtime use)
        std::vector<SkeletonBone> bones;

        // bone Node hierarchy (for editor)
        std::vector<std::unique_ptr<SkeletonBoneNode>> boneNodes;
        SkeletonBoneNode* rootNode = nullptr;

        // name -> index lookup (very useful for animation)
        std::unordered_map<std::string, int> boneMap;

    public:

        inline int GetBoneIndex(const std::string& name) const
        {
            auto it = boneMap.find(name);
            if (it != boneMap.end())
                return it->second;

            return -1;
        }

        inline const SkeletonBone& GetBone(int index) const
        {
            return bones[index];
        }

        inline size_t GetBoneCount() const
        {
            return bones.size();
        }

        inline int GetParentIndex(int index) const
        {
            return bones[index].parentIndex;
        }

        void BuildBoneNodeHierarchy()
        {
            rootNode = nullptr;

            boneNodes.clear();
            boneNodes.resize(bones.size());

            // Create all nodes
            for (size_t i = 0; i < bones.size(); i++)
            {
                boneNodes[i] = std::make_unique<SkeletonBoneNode>();

                boneNodes[i]->name = bones[i].name;
                boneNodes[i]->index = static_cast<int>(i);
            }

            // Build hierarchy
            for (size_t i = 0; i < bones.size(); i++)
            {
                int parentIndex = bones[i].parentIndex;

                SkeletonBoneNode* node = boneNodes[i].get();

                if (parentIndex == -1)
                {
                    rootNode = node;
                }
                else
                {
                    SkeletonBoneNode* parent = boneNodes[parentIndex].get();

                    node->parentNode = parent;
                    parent->childNodes.push_back(node);
                }
            }
        }
    };

    static std::shared_ptr<Skeleton> ReadSkeleton(const std::filesystem::path& path)
    {
        std::ifstream in(path, std::ios::binary);

        if (!in)
            throw std::runtime_error("Failed to open skeleton");

        auto skeleton = std::make_shared<Skeleton>();

        // UUID
        in.read((char*)&skeleton->skeletonID, sizeof(UUID));

        // Name
        uint32_t nameLen;
        in.read((char*)&nameLen, sizeof(uint32_t));

        skeleton->name.resize(nameLen);
        in.read(&skeleton->name[0], nameLen);

        // source path
        uint32_t pathLen;
        in.read((char*)&pathLen, sizeof(uint32_t));

        skeleton->sourcePath.resize(pathLen);
        in.read(&skeleton->sourcePath[0], pathLen);

        // bone count
        uint32_t boneCount;
        in.read((char*)&boneCount, sizeof(uint32_t));

        skeleton->bones.resize(boneCount);


        for (uint32_t i = 0; i < boneCount; i++)
        {
            uint32_t nameLen;
            in.read((char*)&nameLen, sizeof(uint32_t));

            skeleton->bones[i].name.resize(nameLen);
            in.read(&skeleton->bones[i].name[0], nameLen);

            in.read((char*)&skeleton->bones[i].parentIndex, sizeof(int));
            in.read((char*)&skeleton->bones[i].inverseBindMatrix, sizeof(glm::mat4));
            in.read((char*)&skeleton->bones[i].bindMatrix, sizeof(glm::mat4));


            skeleton->boneMap[skeleton->bones[i].name] = i;
        }

        return skeleton;
    }
}
