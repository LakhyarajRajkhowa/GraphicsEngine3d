#pragma once

#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "graphics/opengl/GLSLProgram.h"
#include "graphics/camera/Camera3d.h"
#include "scene/components/Light.h"
#include "graphics/shadowMaps/shadowMap.h"
#include "graphics/shadowMaps/shadowCubeMap.h"
#include "resources/TextureCache.h"
#include "resources/AssetManager.h"

#include "IRenderer.h"
#include "RenderQueue.h"
#include "RenderCommand.h"

namespace Lengine {

    struct RenderFlags {
        bool entitySelected = false;
        bool entityDragged = false;
    };


    class ForwardRenderer : public IRenderer {
    public:
        ForwardRenderer(
            AssetManager& assetmgr

        ) :
            assetManager(assetmgr)
        {
        }

        void Render(
            const RenderContext& ctx
        ) override
        {
            
            if (IRenderer::enableDebugView) RenderScene_debug(ctx);
            else RenderScene(ctx);
        }

        void CollectAndSort(const RenderContext& ctx);
        void FlushTransparentQueue(const RenderContext& ctx,
            bool ueseExternalQueue = false,
            const RenderQueue& externalQueue = RenderQueue{ 512 });

   
    private:
        AssetManager& assetManager;

        float nearPlane = 0.1f;
        float farPlane = 1000.5f;

        RenderQueue   opaqueQueue{ 512 };
        RenderQueue   transparentQueue{ 128 };
        CommandBuffer forwardCommandBuffer{ 2048 };

        ResolvedMaterial resolvePBRMaterial(
            const Material& baseMaterial,
            const MaterialInstance& inst
        );



        void RenderScene(
            const RenderContext& ctx

        );

        void FlushOpaqueQueue(const RenderContext& ctx);

        void lightingPass(
            const RenderContext& ctx,
            std::shared_ptr<GLSLProgram> shader
        );

        void brdfPass(
            const RenderContext& ctx,
            std::shared_ptr<GLSLProgram> shader
        );

        void shadowMapPass(
            const RenderContext& ctx,
            std::shared_ptr<GLSLProgram> shader
        );


        void RenderScene_debug(
            const RenderContext& ctx
        );


        void bindShadowMapUniforms(
            GLSLProgram& shader,
            ShadowMap& shadowMap,
            const TransformComponent& lightTransform,
            const glm::vec3& camPos
        );
        void bindPointShadowUniforms(
            GLSLProgram& shader,
            ShadowCubeMap& shadowCubeMap
        );

        void bindCameraUniforms(
            GLSLProgram& shader,
            const glm::mat4& model,
            Camera3d& editorCamera
        );


        void bindPBRLights(
            GLSLProgram& shader,
            const std::vector<Light>& lights
        );

        void bindPBRMaterial(
            GLSLProgram& shader,
            const ResolvedMaterial& mat
        );


        void bindTexture(
            GLSLProgram& shader,
            AssetManager& assetManager,
            const UUID& texID,
            const bool  use,
            const char* hasUniform,
            const char* samplerUniform,
            GLenum textureUnit
        );

        void drawSubMesh(
            Mesh& sm,
            GLSLProgram& shader
        );



    };
}

