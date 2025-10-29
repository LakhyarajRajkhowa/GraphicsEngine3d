#include "Model.h"

#include <memory>
namespace Lengine {
    void Model::loadModel(const std::string& path, std::shared_ptr<Lengine::Mesh>& mesh) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        Lengine::loadOBJ(path, vertices, indices);
        mesh = std::make_shared<Lengine::Mesh>(vertices, indices);
    }

}