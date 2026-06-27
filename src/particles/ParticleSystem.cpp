#include "ParticleSystem.h"
#include "core/paths.h"
#include <random>
#include <cmath>

using namespace Lengine;

static std::mt19937 s_rng{ std::random_device{}() };

float ParticleSystem::RandRange(float lo, float hi) {
    std::uniform_real_distribution<float> dist(lo, hi);
    return dist(s_rng);
}

int ParticleSystem::RandRangeInt(int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(s_rng);
}

// Samples a random direction within `coneAngleDeg` of `axis`,
// uniformly distributed over the spherical cap (not just linear angle lerp,
// otherwise samples bunch up near the axis).
glm::vec3 ParticleSystem::RandomDirectionInCone(const glm::vec3& axis, float coneAngleDeg) {
    float coneRad = glm::radians(coneAngleDeg);

    float z = RandRange(std::cos(coneRad), 1.0f);   // cos(theta), uniform over cap
    float phi = RandRange(0.0f, glm::two_pi<float>());
    float r = std::sqrt(1.0f - z * z);

    glm::vec3 localDir(r * std::cos(phi), r * std::sin(phi), z);

    // Build a tangent basis around `axis` so localDir's local Z aligns with axis
    glm::vec3 up = glm::abs(axis.y) < 0.99f ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
    glm::vec3 tangent = glm::normalize(glm::cross(up, axis));
    glm::vec3 bitangent = glm::cross(axis, tangent);

    return glm::normalize(
        tangent * localDir.x + bitangent * localDir.y + axis * localDir.z
    );
}

void ParticleSystem::Init() {
    // Compile the billboard shader — same convention as ShadowMap/Skybox owning
    // their own GLSLProgram member directly.
    particleShader.compileShaders(
        Paths::Shaders + "particle.vert",
        Paths::Shaders + "particle.frag"
    );
    particleShader.linkShaders();

    // Unit quad in local space, corners at -0.5..0.5 (billboard expands this in the shader)
    float quadVertices[] = {
        // x,    y
        -0.5f, -0.5f,
         0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f,
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(quadVAO);

    // location 0: per-vertex quad corner
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // instance buffer — allocated empty, refilled with glBufferSubData each frame in Render()
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, pool.size() * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);

    // location 1: instance position (vec3) + size (float) packed as vec4
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
        (void*)offsetof(InstanceData, position));
    glVertexAttribDivisor(1, 1);

    // location 2: instance color (vec4)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
        (void*)offsetof(InstanceData, color));
    glVertexAttribDivisor(2, 1);

    // location 3: instance rotation (float)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
        (void*)offsetof(InstanceData, rotation));
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);

    instanceScratch.reserve(pool.size());
}

void ParticleSystem::SpawnBurst(
    const UUID& emitterAssetID,
    const glm::vec3& origin,
    const glm::vec3& normal
) {
    auto asset = assetManager.GetParticleEmitter(emitterAssetID);
    if (!asset) return;


    glm::vec3 axis = glm::length(normal) > 0.0001f ? glm::normalize(normal) : glm::vec3(0, 1, 0);
    int count = RandRangeInt(asset->burstCountMin, asset->burstCountMax);


    for (int i = 0; i < count; ++i) {
        Particle& p = pool[nextFree];
        nextFree = (nextFree + 1) % pool.size(); // ring buffer — overwrites oldest if pool is full

        glm::vec3 dir = RandomDirectionInCone(axis, asset->coneAngleDeg);
        float     speed = RandRange(asset->speedMin, asset->speedMax);

        p.position = origin;
        p.velocity = dir * speed;
        p.colorStart = asset->colorStart;
        p.colorEnd = asset->colorEnd;
        p.color = asset->colorStart;
        p.sizeStart = asset->sizeStart;
        p.sizeEnd = asset->sizeEnd;
        p.size = asset->sizeStart;
        p.rotation = RandRange(0.0f, glm::two_pi<float>());
        p.age = 0.0f;
        p.lifetime = RandRange(asset->lifetimeMin, asset->lifetimeMax);
        p.gravity = asset->gravity;
        p.drag = asset->drag;
        p.alive = true;
    }
}

void ParticleSystem::Update(float dt) {
    aliveCount = 0;

    for (Particle& p : pool) {
        if (!p.alive) continue;

        p.age += dt;
        if (p.age >= p.lifetime) {
            p.alive = false;
            continue;
        }

        p.velocity.y += p.gravity * dt;
        p.velocity *= glm::clamp(1.0f - p.drag * dt, 0.0f, 1.0f);
        p.position += p.velocity * dt;

        float t = p.NormalizedAge();
        p.size = glm::mix(p.sizeStart, p.sizeEnd, t);
        p.color = glm::mix(p.colorStart, p.colorEnd, t);


        ++aliveCount;
    }
}

void ParticleSystem::Render(const glm::mat4& view, const glm::mat4& projection) {
    instanceScratch.clear();
    for (const Particle& p : pool) {
        if (!p.alive) continue;
        instanceScratch.push_back({ p.position, p.size, p.color, p.rotation });
    }

    if (instanceScratch.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
        instanceScratch.size() * sizeof(InstanceData),
        instanceScratch.data());

    particleShader.use();
    particleShader.setMat4("view", view);
    particleShader.setMat4("projection", projection);

    glBindVertexArray(quadVAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)instanceScratch.size());
    glBindVertexArray(0);


    particleShader.unuse();
}