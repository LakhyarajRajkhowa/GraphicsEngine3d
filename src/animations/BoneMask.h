#pragma once

#include "utils/UUID.h"

namespace Lengine {
	struct BoneMask {
		std::string name;
		UUID id;
		UUID skeletonId;

		std::vector<std::pair<int, std::string>> boneNames;
		std::vector<float> boneMask;
	};

    static std::shared_ptr<BoneMask> LoadBoneMask(
        const std::filesystem::path& filepath)
    {
        std::ifstream file(filepath);

        if (!file.is_open())
            return nullptr;

        auto mask = std::make_shared<BoneMask>();

        std::string line;

        std::getline(file, line);
        mask->name = line.substr(line.find('=') + 1);

        std::getline(file, line);
        mask->id = UUID(
            std::stoull(line.substr(line.find('=') + 1)));

        std::getline(file, line);
        mask->skeletonId = UUID(
            std::stoull(line.substr(line.find('=') + 1)));

        std::getline(file, line); // blank line

        std::getline(file, line);
        size_t boneCount =
            std::stoull(line.substr(line.find('=') + 1));

        mask->boneNames.reserve(boneCount);

        for (size_t i = 0; i < boneCount; i++)
        {
            std::getline(file, line);

            size_t commaPos = line.find(',');

            int boneId =
                std::stoi(line.substr(0, commaPos));

            std::string boneName =
                line.substr(commaPos + 1);

            mask->boneNames.emplace_back(
                boneId,
                boneName);
        }

        std::getline(file, line); // blank line

        std::getline(file, line);
        size_t maskCount =
            std::stoull(line.substr(line.find('=') + 1));

        mask->boneMask.resize(maskCount);

        for (size_t i = 0; i < maskCount; i++)
        {
            std::getline(file, line);

            size_t commaPos = line.find(',');

            int boneId =
                std::stoi(line.substr(0, commaPos));

            float value =
                std::stof(line.substr(commaPos + 1));

            if (boneId >= 0 &&
                boneId < (int)mask->boneMask.size())
            {
                mask->boneMask[boneId] = value;
            }
        }

        return mask;
    }

}
