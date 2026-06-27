#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>


#include "resources/AssetManager.h"
#include "utils/UUID.h"
#include "Particle.h"
#include "ParticleEmitterAsset.h"

namespace Lengine {

    class ParticleSystem {
    public:
        explicit ParticleSystem(AssetManager& assetManager, size_t maxParticles = 4096)
            : assetManager(assetManager), pool(maxParticles) {}

        void Init();
        void Update(float dt);
        void Render(const glm::mat4& view, const glm::mat4& projection);

        // Fire-and-forget burst spawn — call this from script trigger callbacks
        void SpawnBurst(
            const UUID& emitterAssetID,
            const glm::vec3& origin,
            const glm::vec3& normal
        );

        size_t GetAliveCount() const { return aliveCount; }

    private:
        // Tightly packed per-instance data uploaded to GPU each frame
        struct InstanceData {
            glm::vec3 position;   // world position
            float     size;
            glm::vec4 color;
            float     rotation;
        };

        AssetManager& assetManager;

        std::vector<Particle> pool;
        size_t nextFree = 0;   // ring-buffer cursor for overwrite-oldest
        size_t aliveCount = 0;

        std::vector<InstanceData> instanceScratch;

        GLuint quadVAO = 0;
        GLuint quadVBO = 0;
        GLuint instanceVBO = 0;

        GLSLProgram particleShader;

        // -- helpers --
        static float RandRange(float lo, float hi);
        static int   RandRangeInt(int lo, int hi);
        static glm::vec3 RandomDirectionInCone(const glm::vec3& axis, float coneAngleDeg);
    };

}