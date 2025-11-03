#pragma once

#include <string>

#include "Mesh.h"
#include "Model.h"
#include "GLSLProgram.h"
#include "Material.h"
#include "Camera3d.h"
#include "Transform.h"

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
		const glm::mat4& getTransformMatrix() const { return transform.getMatrix(); }
		
		bool isSelected = false;
	private:
		std::string name;
		Mesh* mesh = nullptr;
		Material* material = nullptr;
		Transform transform;
        
	};
}

