#pragma once

#include "utils/UUID.h"
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <memory>

namespace Lengine {

    enum class ParticleBlendMode {
        AlphaBlend,
        Additive
    };

    struct ParticleEmitterAsset {
        UUID        id = UUID::Null;
        std::string name;

        UUID              textureID = UUID::Null;
        ParticleBlendMode blendMode = ParticleBlendMode::AlphaBlend;

        int   burstCountMin = 12;
        int   burstCountMax = 20;

        float lifetimeMin = 0.4f;
        float lifetimeMax = 0.9f;

        float speedMin = 2.0f;
        float speedMax = 6.0f;

        float coneAngleDeg = 35.0f;

        float sizeStart = 0.08f;
        float sizeEnd = 0.02f;

        glm::vec4 colorStart = { 0.5f, 0.0f, 0.0f, 1.0f };
        glm::vec4 colorEnd = { 0.2f, 0.0f, 0.0f, 0.0f };

        float gravity = -9.8f;
        float drag = 1.5f;
    };

    // "r,g,b,a" -> vec4, mirrors the comma-split style BoneMask uses for "id,name"
    static glm::vec4 ParseVec4Csv(const std::string& csv)
    {
        glm::vec4 v(0.0f);
        std::stringstream ss(csv);
        std::string token;
        int i = 0;
        while (std::getline(ss, token, ',') && i < 4)
            v[i++] = std::stof(token);
        return v;
    }

    static std::shared_ptr<ParticleEmitterAsset> LoadParticleEmitter(
        const std::filesystem::path& filepath)
    {
        std::ifstream file(filepath);

        if (!file.is_open())
            return nullptr;

        auto asset = std::make_shared<ParticleEmitterAsset>();
        std::string line;

        std::getline(file, line);
        asset->name = line.substr(line.find('=') + 1);

        std::getline(file, line);
        asset->id = UUID(std::stoull(line.substr(line.find('=') + 1)));

        std::getline(file, line);
        asset->textureID = UUID(std::stoull(line.substr(line.find('=') + 1)));

        std::getline(file, line);
        asset->blendMode = static_cast<ParticleBlendMode>(
            std::stoi(line.substr(line.find('=') + 1)));

        std::getline(file, line); // blank line

        std::getline(file, line);
        asset->burstCountMin = std::stoi(line.substr(line.find('=') + 1));

        std::getline(file, line);
        asset->burstCountMax = std::stoi(line.substr(line.find('=') + 1));

        std::getline(file, line);
        asset->lifetimeMin = std::stof(line.substr(line.find('=') + 1));

        std::getline(file, line);
        asset->lifetimeMax = std::stof(line.substr(line.find('=') + 1));

        std::getline(file, line);
        asset->speedMin = std::stof(line.substr(line.find('=') + 1));

        std::getline(file, line);
        asset->speedMax = std::stof(line.substr(line.find('=') + 1));

        std::getline(file, line);
        asset->coneAngleDeg = std::stof(line.substr(line.find('=') + 1));

        std::getline(file, line); // blank line

        std::getline(file, line);
        asset->sizeStart = std::stof(line.substr(line.find('=') + 1));

        std::getline(file, line);
        asset->sizeEnd = std::stof(line.substr(line.find('=') + 1));

        std::getline(file, line);
        asset->colorStart = ParseVec4Csv(line.substr(line.find('=') + 1));

        std::getline(file, line);
        asset->colorEnd = ParseVec4Csv(line.substr(line.find('=') + 1));

        std::getline(file, line); // blank line

        std::getline(file, line);
        asset->gravity = std::stof(line.substr(line.find('=') + 1));

        std::getline(file, line);
        asset->drag = std::stof(line.substr(line.find('=') + 1));

        return asset;
    }

}