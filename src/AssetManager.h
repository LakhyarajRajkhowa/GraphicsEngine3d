#pragma once
#include <unordered_map>
#include <memory>
#include "GLSLProgram.h"
#include "Mesh.h"
#include "Model.h"


namespace Lengine {
	class AssetManager {
	private:
		std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
		std::unordered_map<std::string, std::shared_ptr<GLSLProgram>> shaders;
	public:
        Mesh* loadMesh(const std::string& name, const std::string& path);
        Mesh* getMesh(const std::string& name);


        GLSLProgram* loadShader(const std::string& name,
            const std::string& vertPath,
            const std::string& fragPath);
		GLSLProgram* getShader(const std::string& name);
	};
}