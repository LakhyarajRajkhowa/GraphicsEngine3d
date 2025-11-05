#include "AssetManager.h"
#include <iostream>

using namespace Lengine;


Mesh* AssetManager::getMesh(const std::string& name) {
    return meshes[name].get();
}

Mesh* AssetManager::loadMesh(const std::string& name, const std::string& path) {
    if (meshes.count(name)) return meshes[name].get();

    std::shared_ptr<Mesh> meshPtr;
    Model model;
    model.loadModel(name, path, meshPtr);

    meshes[name] = std::move(meshPtr);

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

GLTexture* AssetManager::loadTexture(const std::string& name , const std::string& path) {

    if (textures.count(name)) return textures[name].get();
    auto texture = std::make_unique<GLTexture>();

    *texture = textureCache.getTexture(path);
    
    textures[name] = std::move(texture);
    return textures[name].get();
}
