#pragma once
#include <glm/glm.hpp>
#include <vector>

struct ForceCommand
{
    enum class Type { Force, Torque };
    glm::vec3 value;
    Type type;
    int mode;
};

struct RigidbodyComponent
{
    RigidbodyComponent() = default;
    explicit RigidbodyComponent(
        const bool useGravity,
        const bool isKinematic = false,
        const float mass = 1.0f,
        const float linearDamping = 0.1f,
        const float angularDamping = 0.05f,
        const glm::vec3 linearVelocity = glm::vec3(0.0f),
        const glm::vec3 angularVelocity = glm::vec3(0.0f)
    ) :
        useGravity(useGravity),
        isKinematic(isKinematic),
        mass(mass),
        linearDamping(linearDamping),
        angularDamping(angularDamping),
        linearVelocity(linearVelocity),
        angularVelocity(angularVelocity)
    {}

    float mass = 1.0f;
    float linearDamping = 0.1f;
    float angularDamping = 0.05f;

    bool useGravity = true;
    bool isKinematic = false;

    bool lockLinearX = false, lockLinearY = false, lockLinearZ = false;
    bool lockAngularX = false, lockAngularY = false, lockAngularZ = false;

    glm::vec3 linearVelocity = glm::vec3(0.0f);
    glm::vec3 angularVelocity = glm::vec3(0.0f);

    std::vector<ForceCommand> pendingForces;


    bool dirty = true;
};