#pragma once
#include <unordered_map>
#include <memory>
#include "../graphics/opengl/GLSLProgram.h"
#include "../graphics/opengl/GLTexture.h"
#include "../graphics/geometry/Mesh.h"
#include "../graphics/geometry/Model.h"
#include "../resources/TextureCache.h"

#include "../utils/metaFileSystem.h"

namespace Lengine {
	class AssetManager {
	private:
		std::unordered_map<UUID, std::shared_ptr<Mesh>> meshes;
		std::unordered_map<std::string, std::shared_ptr<GLSLProgram>> shaders;
		std::unordered_map<UUID, std::shared_ptr<GLTexture>> textures;

		TextureCache textureCache;
		
	public:
		UUID getUUID(const std::string& name);
		UUID loadMesh(const std::string& name, const std::string& path);
		Mesh* getMesh(const UUID& id);



		GLTexture* getTexture(const std::string& name);
		GLTexture* loadTexture(const std::string& name, const std::string& path);


        GLSLProgram* loadShader(const std::string& name,
            const std::string& vertPath,
            const std::string& fragPath);
		GLSLProgram* getShader(const std::string& name);
	};
}