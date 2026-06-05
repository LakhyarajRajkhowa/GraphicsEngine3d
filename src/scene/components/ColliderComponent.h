#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <PxPhysicsAPI.h>

namespace Lengine {

    struct ColliderShape
    {
        enum class Type
        {
            Box,
            Sphere,
            Capsule
        };

        Type type = Type::Box;

        glm::vec3 size = { 1,1,1 };
        float radius = 0.5f;
        float height = 1.0f;

        bool isTrigger = false;

        physx::PxShape* runtimeShape = nullptr;

        bool dirty = true;
    };

    // ColliderComponent.h

    struct ColliderComponent
    {
        ColliderComponent() = default;

        // Pass any number of shapes directly
        ColliderComponent(std::initializer_list<ColliderShape> shapeList)
            : shapes(shapeList)
        {}

        // Convenience static factories so you don't need to fill ColliderShape manually
        static ColliderComponent Box(
            glm::vec3 size = { 1, 1, 1 },
            bool      isTrigger = false)
        {
            ColliderShape s;
            s.type = ColliderShape::Type::Box;
            s.size = size;
            s.isTrigger = isTrigger;
            return ColliderComponent({ s });
        }

        static ColliderComponent Sphere(
            float radius = 0.5f,
            bool  isTrigger = false)
        {
            ColliderShape s;
            s.type = ColliderShape::Type::Sphere;
            s.radius = radius;
            s.isTrigger = isTrigger;
            return ColliderComponent({ s });
        }

        static ColliderComponent Capsule(
            float radius = 0.5f,
            float height = 1.0f,
            bool  isTrigger = false)
        {
            ColliderShape s;
            s.type = ColliderShape::Type::Capsule;
            s.radius = radius;
            s.height = height;
            s.isTrigger = isTrigger;
            return ColliderComponent({ s });
        }

        ColliderComponent& AddShape(const ColliderShape& shape)
        {
            shapes.push_back(shape);
            return *this;
        }

        std::vector<ColliderShape> shapes;
    };

}