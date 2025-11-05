#pragma once
#include <unordered_map>
#include <memory>
#include "GLSLProgram.h"
#include "Mesh.h"
#include "Model.h"
#include "GLTexture.h"
#include "TextureCache.h"

namespace Lengine {
	class AssetManager {
	private:
		std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
		std::unordered_map<std::string, std::shared_ptr<GLSLProgram>> shaders;
		std::unordered_map<std::string, std::shared_ptr<GLTexture>> textures;

		TextureCache textureCache;
		
	public:
        
		Mesh* loadMesh(const std::string& name, const std::string& path);
		Mesh* getMesh(const std::string& name);

		GLTexture* getTexture(const std::string& name);
		GLTexture* loadTexture(const std::string& name, const std::string& path);


        GLSLProgram* loadShader(const std::string& name,
            const std::string& vertPath,
            const std::string& fragPath);
		GLSLProgram* getShader(const std::string& name);
	};
}