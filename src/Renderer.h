#pragma once
#include "Scene.h"
#include "GLSLProgram.h"
#include "Camera3d.h"

namespace Lengine {
    class Renderer {
    public:
        void renderScene(const Scene& scene, Camera3d& camera);
    };
}
