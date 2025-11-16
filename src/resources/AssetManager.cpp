#include "AssetManager.h"

using namespace Lengine;

void AssetManager::LoadAllMetaFiles(const fs::path& root)
{
    if (!fs::exists(root))
        return;

    for (auto& entry : fs::recursive_directory_iterator(root))
    {
        if (!entry.is_regular_file())
            continue;

        const fs::path& metaPath = entry.path();

        // Must end with ".meta"
        if (metaPath.extension() != ".meta")
            continue;

        // Load meta (contains uuid, type, source)
        MetaFile meta = MetaFileSystem::Load(metaPath.string());

        // Get original file path (remove ".meta")
        fs::path assetPath = metaPath;
        assetPath.replace_extension("");    // removes .meta → "mesh.obj"

        if (assetPath.extension() == ".obj") {
           // loadMesh(ExtractNameFromPath(assetPath.string()), assetPath.string());
        }
    }
}


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
void AssetManager::loadMesh2(const UUID& uuid, const std::string& path)
{
    std::string meshName = ExtractNameFromPath(path);
    UUID id = uuid;
    std::string newPath = StripQuotes(path);

    std::shared_ptr<Mesh> ptr;
    Model model;
    model.loadModel(meshName, newPath, ptr);

    meshes[uuid] = ptr;
}


UUID AssetManager::importMesh(const std::string& path) {
    MetaFile meta;
    
    if (!MetaFileSystem::HasMeta(path)) {
        meta.uuid = UUID();
        meta.type = "mesh";
        meta.source = NormalizePath(path);

        MetaFileSystem::Save(path, meta);
    }
    else {
        meta = MetaFileSystem::Load(path);
    }

    return meta.uuid;
}

UUID AssetManager::loadMesh(const std::string& name, const std::string& path) {
    MetaFile meta;

     
    if (!MetaFileSystem::HasMeta(path)) {
        meta.uuid = UUID();
        meta.type = "mesh";
        meta.source = ExtractFileNameFromPath(path);

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
        meta.source = ExtractFileNameFromPath(path);
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
