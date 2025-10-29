#pragma once

#include <string>

#include "Mesh.h"
#include "Model.h"
#include "GLSLProgram.h"
#include "Camera3d.h"
#include "Transform.h"

namespace Lengine {
	class Entity {
	public:
		std::string name;
	    Mesh* mesh = nullptr;
		GLSLProgram* shader = nullptr;
		Transform transform;              

		Entity(const std::string& n, Mesh* m, GLSLProgram* s)
			: name(n), mesh(m), shader(s) {
		}

		void draw(Camera3d& camera);
		const std::string& getName() const { return name; }
		Mesh* getMesh() const { return mesh; }
		GLSLProgram* getShader() const { return shader; }
		const glm::mat4& getTransformMatrix() const { return transform.getMatrix(); }
	private:
        
	};
}

