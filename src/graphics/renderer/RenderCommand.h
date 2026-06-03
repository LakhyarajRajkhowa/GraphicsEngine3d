#pragma once

#include <vector>
#include <memory>
#include <string>
#include <functional>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "graphics/opengl/GLSLProgram.h"
#include "graphics/geometry/Mesh.h"
#include "graphics/material/Material.h"
#include "resources/AssetManager.h"
#include "RenderQueue.h"
#include "RenderContext.h"

namespace Lengine {


    struct IRenderCommand {
        virtual ~IRenderCommand() = default;
        virtual void Execute() = 0;
    };


    struct UseShaderCommand : IRenderCommand {
        GLSLProgram* shader = nullptr;

        explicit UseShaderCommand(GLSLProgram* s) : shader(s) {}

        void Execute() override {
            if (shader) shader->use();
        }
    };

    struct UnuseShaderCommand : IRenderCommand {
        GLSLProgram* shader = nullptr;

        explicit UnuseShaderCommand(GLSLProgram* s) : shader(s) {}

        void Execute() override {
            if (shader) shader->unuse();
        }
    };

    struct SetCameraUniformsCommand : IRenderCommand {
        GLSLProgram* shader = nullptr;
        glm::mat4    view = glm::mat4(1.0f);
        glm::mat4    projection = glm::mat4(1.0f);
        glm::vec3    cameraPos = glm::vec3(0.0f);

        SetCameraUniformsCommand(
            GLSLProgram* s,
            const glm::mat4& v,
            const glm::mat4& p,
            const glm::vec3& cp)
            : shader(s), view(v), projection(p), cameraPos(cp) {}

        void Execute() override {
            if (!shader) return;
            shader->setMat4("view", view);
            shader->setMat4("projection", projection);
            shader->setVec3("cameraPos", cameraPos);
        }
    };


    struct SetModelMatrixCommand : IRenderCommand {
        GLSLProgram* shader = nullptr;
        glm::mat4    model = glm::mat4(1.0f);

        SetModelMatrixCommand(GLSLProgram* s, const glm::mat4& m)
            : shader(s), model(m) {}

        void Execute() override {
            if (shader) shader->setMat4("model", model);
        }
    };


    struct BindPBRMaterialCommand : IRenderCommand {
        GLSLProgram* shader = nullptr;
        ResolvedMaterial material;
        AssetManager* assetManager = nullptr;

        BindPBRMaterialCommand(
            GLSLProgram* s,
            const ResolvedMaterial& mat,
            AssetManager* am)
            : shader(s), material(mat), assetManager(am) {}

        void Execute() override {
            if (!shader || !assetManager) return;

            shader->setVec3("material.albedo", material.albedo);
            shader->setFloat("material.metallic", material.metallic);
            shader->setFloat("material.roughness", material.roughness);
            shader->setFloat("material.ao", material.ao);
            shader->setFloat("material.normalStrength", material.normalStrength);
            shader->setFloat("material.opacity", material.opacity);

            bindTex(material.map_albedo,
                material.map_albedo != UUID::Null,   
                "material.hasAlbedoMap", "material.albedoMap",
                GL_TEXTURE0 + (unsigned)TextureUnit::Albedo);

            bindTex(material.map_normal,
                material.map_normal != UUID::Null,
                "material.hasNormalMap", "material.normalMap",
                GL_TEXTURE0 + (unsigned)TextureUnit::Normal);

            bindTex(material.map_ao,
                material.map_ao != UUID::Null,
                "material.hasAOMap", "material.aoMap",
                GL_TEXTURE0 + (unsigned)TextureUnit::AO);

            bindTex(material.map_metallic,
                material.map_metallic != UUID::Null,
                "material.hasMetallicMap", "material.metallicMap",
                GL_TEXTURE0 + (unsigned)TextureUnit::Metallic);

            bindTex(material.map_roughness,
                material.map_roughness != UUID::Null,
                "material.hasRoughnessMap", "material.roughnessMap",
                GL_TEXTURE0 + (unsigned)TextureUnit::Roughness);

            bindTex(material.map_metallicRoughness,
                material.map_metallicRoughness != UUID::Null,
                "material.hasMetallicRoughnessMap", "material.metallicRoughnessMap",
                GL_TEXTURE0 + (unsigned)TextureUnit::MetallicRoughness);
        }

    private:
        void bindTex(
            const UUID& texID,
            bool        useFlag,
            const char* hasUniform,
            const char* samplerUniform,
            GLenum      unit)
        {
            bool has = (texID != UUID::Null && useFlag);
            shader->setBool(hasUniform, has);
            glActiveTexture(unit);
            if (has) {
                GLTexture* tex = assetManager->getTexture(texID);
                glBindTexture(GL_TEXTURE_2D, tex ? tex->id : 0);
                shader->setInt(samplerUniform, unit - GL_TEXTURE0);
            }
            else {
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
    };


    struct SetSkeletonCommand : IRenderCommand {
        GLSLProgram* shader = nullptr;
        std::vector<glm::mat4>    bones;       
        std::vector<int>          palette;    

        SetSkeletonCommand(
            GLSLProgram* s,
            const std::vector<glm::mat4>& allBones,
            const std::vector<int>& pal)
            : shader(s), palette(pal)
        {
            bones.resize(pal.size());
            for (size_t i = 0; i < pal.size(); ++i)
                bones[i] = allBones[pal[i]];
        }

        void Execute() override {
            if (!shader) return;
            shader->setBool("useSkeleton", true);
            for (int i = 0; i < (int)bones.size(); ++i)
                shader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", bones[i]);
        }
    };


    struct SetBoolCommand : IRenderCommand {
        GLSLProgram* shader = nullptr;
        std::string  uniform;
        bool         value = false;

        SetBoolCommand(GLSLProgram* s, std::string u, bool v)
            : shader(s), uniform(std::move(u)), value(v) {}

        void Execute() override {
            if (shader) shader->setBool(uniform.c_str(), value);
        }
    };


    struct DrawMeshCommand : IRenderCommand {
        Mesh* mesh = nullptr;

        explicit DrawMeshCommand(Mesh* m) : mesh(m) {}

        void Execute() override {
            if (mesh) mesh->draw();
        }
    };

    struct EnableBlendCommand : IRenderCommand {
        void Execute() override {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthFunc(GL_LEQUAL); 
        }
    };

    struct DisableBlendCommand : IRenderCommand {
        void Execute() override {
            glDisable(GL_BLEND);
            glDepthFunc(GL_LESS);   
        }
    };


    class CommandBuffer {
    public:
        // #RenderCommands = #RenderItems * #Commands/Item 
        explicit CommandBuffer(size_t reserve = 2048)
        {
            commands.reserve(reserve);
        }

        void Clear()
        {
            commands.clear();
        }

        template<typename T, typename... Args>
        void Record(Args&&... args)
        {
            commands.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        }


        void Execute()
        {
            for (auto& cmd : commands)
                cmd->Execute();
        }

        size_t Size() const { return commands.size(); }

    private:
        std::vector<std::unique_ptr<IRenderCommand>> commands;
    };


    struct GeometryQueueFlusher {
        static void Flush(
            const RenderQueue& queue,
            GLSLProgram* geomShader,
            const RenderContext& ctx,
            AssetManager& assetManager,
            CommandBuffer& buffer)
        {

            buffer.Record<UseShaderCommand>(geomShader);

            buffer.Record<SetCameraUniformsCommand>(
                geomShader,
                ctx.cameraView,
                ctx.cameraProjection,
                ctx.cameraPos);

            for (const RenderItem& item : queue.GetItems())
            {
                if (!item.mesh) continue;

                buffer.Record<SetModelMatrixCommand>(geomShader, item.modelMatrix);
                buffer.Record<BindPBRMaterialCommand>(geomShader, item.material, &assetManager);

                if (item.hasSkeleton && item.boneMatrices && item.bonePalette)
                    buffer.Record<SetSkeletonCommand>(
                        geomShader, *item.boneMatrices, *item.bonePalette);
                else
                    buffer.Record<SetBoolCommand>(geomShader, "useSkeleton", false);

                buffer.Record<DrawMeshCommand>(item.mesh);
            }

            buffer.Record<UnuseShaderCommand>(geomShader);

        }
    };

} // namespace Lengine

