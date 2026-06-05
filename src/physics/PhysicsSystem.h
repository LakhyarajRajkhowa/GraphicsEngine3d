#pragma once

#include <PxPhysicsAPI.h>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace physx;

#include "scene/scene.h"

namespace Lengine {

    //  PxForceMode 
    enum class ForceMode
    {
        Force,          // Continuous (Newtons, mass-dependent)
        Impulse,        // Instant    (kg*m/s,  mass-dependent)
        VelocityChange, // Instant    (m/s,     mass-independent)
        Acceleration    // Continuous (m/s^2,   mass-independent)
    };

    struct PhysicsActor
    {
        enum class Type { STATIC, DYNAMIC, KINEMATIC };

        PxRigidActor* actor = nullptr;
        Type          type = Type::STATIC;
    };


    class PhysicsSystem
    {

    public:

        static bool dirty;

        PhysicsSystem() = default;
        ~PhysicsSystem() = default;

        static PhysicsSystem& getInstance();

        void Init(Scene& scene);
        void InitForScene(Scene& scene);
        void UpdateRuntime(float dt, ComponentStorage<TransformComponent>& transforms);
        void ClearScene();
        void Shutdown();

        // Sync game-side transforms into PhysX 
        void SyncTransformsToPhysX(ComponentStorage<TransformComponent>& transforms);

        // Accessors
        PxPhysics* GetPhysics() { return physics; }
        PxScene* GetScene() { return physxScene; }
        PxMaterial* GetDefaultMaterial() { return material; }

        std::unordered_map<Entity, std::unique_ptr<PhysicsActor>>& GetActors() { return actors; }
        const std::unordered_map<Entity, std::unique_ptr<PhysicsActor>>& GetActors() const { return actors; }

        // =====================================================================
        //  Collider API
        // =====================================================================
        void AddCollider(Entity entity, ColliderComponent& col, ColliderShape::Type type);
        void DeleteColliderShape(Entity entity, ColliderComponent& col, size_t shapeIndex);
        void DeleteCollider(Entity entity, ColliderComponent& col);
        void DeleteRigidBody(Entity entity, ColliderComponent* col);

        // =====================================================================
        //  Rigidbody — structural setters
        //  Safe at any time: update component + push to PhysX if actor exists
        // =====================================================================
        void SetMass(Entity e, float mass);
        void SetGravityEnabled(Entity e, bool enabled);
        void SetKinematic(Entity e, bool kinematic);
        void SetLinearDamping(Entity e, float damping);
        void SetAngularDamping(Entity e, float damping);
        void SetLinearLock(Entity e, bool x, bool y, bool z);
        void SetAngularLock(Entity e, bool x, bool y, bool z);

        // =====================================================================
        //  Rigidbody — velocity
        //  Direct read/write; no-op / zero-return if entity has no dynamic actor
        // =====================================================================
        void      SetLinearVelocity(Entity e, glm::vec3 velocity);
        void      SetAngularVelocity(Entity e, glm::vec3 velocity);
        glm::vec3 GetLinearVelocity(Entity e) const;
        glm::vec3 GetAngularVelocity(Entity e) const;

        // =====================================================================
        //  Rigidbody — forces
        //  Applied immediately; no-op if entity has no dynamic actor yet
        // =====================================================================
        void AddForce(Entity e, glm::vec3 force, ForceMode mode = ForceMode::Force);
        void AddTorque(Entity e, glm::vec3 torque, ForceMode mode = ForceMode::Force);
        void ClearForces(Entity e);

        // =====================================================================
        //  Rigidbody — sleep
        // =====================================================================
        void WakeUp(Entity e);
        void PutToSleep(Entity e);
        bool IsSleeping(Entity e) const;



        // =========================================================================
        //  Private data
        // =========================================================================
    private:

        PxFoundation* foundation = nullptr;
        PxPhysics* physics = nullptr;
        PxDefaultCpuDispatcher* dispatcher = nullptr;
        PxScene* physxScene = nullptr;
        PxMaterial* material = nullptr;
        PxPvd* pvd = nullptr;

        std::unordered_map<Entity, std::unique_ptr<PhysicsActor>> actors;

        std::unordered_set<Entity> pendingColliderAdded;
        std::unordered_set<Entity> pendingColliderRemoved;
        std::unordered_set<Entity> pendingRigidbodyAdded;
        std::unordered_set<Entity> pendingRigidbodyRemoved;

        Registry* registry = nullptr;

        static constexpr float fixedDeltaTime = 1.0f / 120.0f;

        // =========================================================================
        //  Private methods
        // =========================================================================
    private:

        PxRigidDynamic* getDynamicActor(Entity e) const;

        // Deferred component add/remove processing
        void flushPending();

        // Actor construction / destruction
        void buildColliderActor(Entity entity, ColliderComponent& col);
        void buildRigidbodyActor(Entity entity, RigidbodyComponent& rb);
        void teardownCollider(Entity entity, ColliderComponent& col);
        void teardownRigidbody(Entity entity, ColliderComponent* col);

        void drainPendingCommands(Entity e, PxRigidDynamic* dyn, RigidbodyComponent& rb);
       

        // Pushes ALL structural RigidbodyComponent fields into an existing PxRigidDynamic
        void syncRigidbodyProperties(PxRigidDynamic* actor, const RigidbodyComponent& rb);

        // Write PhysX poses back to ECS transforms each frame
        void updateTransforms(ComponentStorage<TransformComponent>& transforms);

        void createGroundPlane();

        // Returns the PxRigidDynamic for entity, or nullptr if static / absent

        // Maps our ForceMode to PxForceMode
        static PxForceMode::Enum toPxForceMode(ForceMode mode);
    };


    // Utility: convert PhysX transform to GLM mat4
    inline glm::mat4 PxToGLM(const physx::PxTransform& t)
    {
        physx::PxMat44 m(t);
        return glm::mat4(
            m.column0.x, m.column0.y, m.column0.z, m.column0.w,
            m.column1.x, m.column1.y, m.column1.z, m.column1.w,
            m.column2.x, m.column2.y, m.column2.z, m.column2.w,
            m.column3.x, m.column3.y, m.column3.z, m.column3.w
        );
    }

} // namespace Lengine