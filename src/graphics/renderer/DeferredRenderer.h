#pragma once


#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "graphics/opengl/GLSLProgram.h"
#include "graphics/camera/Camera3d.h"
#include "scene/components/Light.h"
#include "graphics/renderer/IRenderer.h"
#include "graphics/shadowMaps/shadowMap.h"
#include "graphics/shadowMaps/shadowCubeMap.h"
#include "resources/AssetManager.h"
#include "graphics/Framebuffers/Framebuffer.h"
#include "graphics/geometry/FullScreenQuad.h"

#include "RenderQueue.h"
#include "RenderCommand.h"

namespace Lengine {

    class DeferredRenderer {
    public:
        DeferredRenderer(AssetManager& assetmgr)
            : assetManager(assetmgr)
        {
            fullscreenQuad.Init();
        }

        void RenderGeometry(const RenderContext& ctx);
        void RenderLighting(const RenderContext& ctx, const Framebuffer& gBuffer);

        RenderQueue& GetTransparentQueue() { return transparentQueue; }

    private:
        AssetManager& assetManager;
        FullscreenQuad fullscreenQuad;

        float nearPlane = 0.1f;
        float farPlane = 1000.5f;


        RenderQueue   opaqueQueue{ 512 };
        RenderQueue   transparentQueue{ 512 };
        CommandBuffer geometryCommandBuffer{ 2048 };

        void bindShadowMapUniforms(GLSLProgram& shader, ShadowMap& shadowMap,
            const TransformComponent& lightTransform,
            const glm::vec3& camPos);
        void bindPointShadowUniforms(GLSLProgram& shader, ShadowCubeMap& shadowCubeMap);
        void bindCameraUniforms(GLSLProgram& shader, const glm::mat4& model, Camera3d& cam);
        void bindPBRLights(GLSLProgram& shader, const std::vector<Light>& lights);
        void bindPBRMaterial(GLSLProgram& shader, const ResolvedMaterial& mat);
        void bindTexture(GLSLProgram& shader, AssetManager& am, const UUID& texID,
            bool use, const char* hasUniform, const char* samplerUniform,
            GLenum textureUnit);
        void drawSubMesh(Mesh& sm, GLSLProgram& shader);
        void RenderScene_debug(const RenderContext& ctx);
    };

} // namespace Lengine