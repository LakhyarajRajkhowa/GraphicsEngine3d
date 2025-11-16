#pragma once
#include <unordered_map>
#include <memory>
#include <filesystem>
#include "../graphics/opengl/GLSLProgram.h"
#include "../graphics/opengl/GLTexture.h"
#include "../graphics/geometry/Mesh.h"
#include "../graphics/geometry/Model.h"
#include "../resources/TextureCache.h"

#include "../utils/metaFileSystem.h"


namespace fs = std::filesystem;

namespace Lengine {

	class AssetManager {
	private:
		std::unordered_map<UUID, std::shared_ptr<Mesh>> meshes;
		std::unordered_map<std::string, std::shared_ptr<GLSLProgram>> shaders;
		std::unordered_map<UUID, std::shared_ptr<GLTexture>> textures;


		TextureCache textureCache;
		
	public:
		AssetManager::AssetManager()
		{
			LoadAllMetaFiles("../TestGameFolder/assets");
		}
		void LoadAllMetaFiles(const fs::path& root);
		UUID getUUID(const std::string& name);
		void loadMesh2(const UUID& uuid, const std::string& path);
		UUID loadMesh(const std::string& name, const std::string& path);
		Mesh* getMesh(const UUID& id);

		UUID importMesh(const std::string& path);

		GLTexture* getTexture(const std::string& name);
		GLTexture* loadTexture(const std::string& name, const std::string& path);


        GLSLProgram* loadShader(const std::string& name,
            const std::string& vertPath,
            const std::string& fragPath);
		GLSLProgram* getShader(const std::string& name);
	};
}