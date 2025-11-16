#include "Model.h"

#include <memory>
namespace Lengine {
   
    void Model::loadModel(const std::string& name, const std::string& path, std::shared_ptr<Lengine::Mesh>& mesh) {
        mesh = std::make_shared<Lengine::Mesh>();
        mesh->name = name;
        
        
        assimpLoader(path, *mesh);
        for (auto& sm : mesh->subMeshes)
            sm.setupMesh();
        

    }

    void Model::StartAsyncLoad(const std::string& path, std::shared_ptr<Lengine::Mesh> mesh)
    {
        if (isLoading) return;

        isLoading = true;
        meshReady = false;
        loadingProgress = 0.0f;

        loadingThread = std::thread([path, mesh]() {

            assimpLoader(path, *mesh); // background thread safe
            isLoading = false;
            meshReady = true;
            for (auto& sm : mesh->subMeshes)
                sm.setupMesh();
            std::cout << "Loaded: " << path << std::endl;
            });

        loadingThread.detach();
    }


}