#include "AssetManager.h"

using namespace Lengine;

Mesh* AssetManager::loadMesh(const std::string& name, const std::string& path) {
    if (meshes.count(name)) return meshes[name].get();

    std::shared_ptr<Mesh> meshPtr;
    Model model;
    model.loadModel(path, meshPtr);

    meshes[name] = std::move(meshPtr);
    return meshes[name].get();
}

Mesh* AssetManager::getMesh(const std::string& name) {
    return meshes[name].get();
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