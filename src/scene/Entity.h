#pragma once

#include <string>

#include "../graphics/geometry/Mesh.h"
#include "../graphics/geometry/Model.h"
#include "../graphics/opengl/GLSLProgram.h"
#include "../graphics/material/Material.h"
#include "../graphics/camera/Camera3d.h"
#include "../scene/Transform.h"

#include "../utils/UUID.h"

namespace Lengine {
	class Entity {
	public:
		Entity(const std::string& n, UUID mID, Material* mat)
			: name(n), meshID(mID), material(mat) {
		}
		
		const std::string& getName() const { return name; }
		void setName(const std::string& newName) { name = newName; }
		UUID getMeshID() const { return meshID; }
		void setMeshID(const UUID& id) { meshID = id; }

		
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
		UUID meshID;
		Material* material = nullptr;
		Transform transform;
        
	};
}

