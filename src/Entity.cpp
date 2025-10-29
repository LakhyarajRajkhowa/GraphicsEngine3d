#include "Entity.h"

namespace Lengine {
    void Entity::draw(Camera3d& camera) {
        if (!mesh || !shader) return;

        shader->use();
        shader->setMat4("model", transform.getMatrix());
        shader->setMat4("view", camera.getViewMatrix());
        shader->setMat4("projection", camera.getProjectionMatrix());
        shader->setVec3("cameraPos", camera.getCameraPosition());

        mesh->draw();

        shader->unuse();
    }
}
