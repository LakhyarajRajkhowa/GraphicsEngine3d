#include "AssetManager.h"

using namespace Lengine;


Mesh* AssetManager::getMesh(const UUID& id) {
    return meshes[id].get();
}
UUID AssetManager::getUUID(const std::string& name) {
    for (auto& [uuid, mesh] : meshes)
    {
        if (mesh && mesh->name == name)
            return uuid;
    }

    return UUID::Null;
}

UUID AssetManager::loadMesh(const std::string& name, const std::string& path) {
    MetaFile meta;

    if (!MetaFileSystem::HasMeta(path)) {
        meta.uuid = UUID();
        meta.type = "texture";
        MetaFileSystem::Save(path, meta);
    }
    else {
        meta = MetaFileSystem::Load(path);
    }

    UUID id = meta.uuid;

    std::shared_ptr<Mesh> meshPtr;
    Model model;
    model.loadModel(name, path, meshPtr);
    meshes[id] = meshPtr;

    return id;
}


GLSLProgram* AssetManager::loadShader(const std::string& name,
    const std::string& vert,
    const std::string& frag)
{
    if (shaders.count(name)) return shaders[name].get();

    auto shader = std::make_unique<GLSLProgram>();
    shader->compileShaders(vert.c_str(), frag.c_str());
    shader->linkShaders();

    shaders[name] = std::move(shader);

    return shaders[name].get();
}

GLSLProgram* AssetManager::getShader(const std::string& name) {
    return shaders[name].get();
}

GLTexture* AssetManager::loadTexture(const std::string& name , const std::string& path) {


    MetaFile meta;

    if (!MetaFileSystem::HasMeta(path)) {
        meta.uuid = UUID();
        meta.type = "texture";
        MetaFileSystem::Save(path, meta);
    }
    else {
        meta = MetaFileSystem::Load(path);
    }

    UUID id = meta.uuid;
    std::shared_ptr<GLTexture> tex = std::make_shared<GLTexture>();
    *tex = textureCache.getTexture(path);

    textures[id] = tex;
    return tex.get();
 
}
