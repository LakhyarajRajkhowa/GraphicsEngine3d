#pragma once

#include <string>

#include "../graphics/geometry/Mesh.h"
#include "../graphics/geometry/Model.h"
#include "../graphics/opengl/GLSLProgram.h"
#include "../graphics/material/Material.h"
#include "../graphics/camera/Camera3d.h"
#include "../scene/Transform.h"

namespace Lengine {
	class Entity {
	public:
		Entity(const std::string& n, Mesh* m, Material* mat)
			: name(n), mesh(m), material(mat) {
		}
		
		const std::string& getName() const { return name; }
		Mesh* getMesh() const { return mesh; }

		
		Material* getMaterial() const { return material; }
		Transform& getTransform() {
			return transform;
		}
		const Transform& getTransform() const {
			return transform;
		};
		const glm::mat4& getTransformMatrix() const { return transform.getMatrix(); }
		
		bool isSelected = false;
		bool isMovable = true;
	private:
		std::string name;
		Mesh* mesh = nullptr;
		Material* material = nullptr;
		Transform transform;
        
	};
}

