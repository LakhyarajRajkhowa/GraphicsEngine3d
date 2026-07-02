#include "shadowCubeMap.h"

using namespace Lengine;

void ShadowCubeMap::Init(){

	shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);

	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_RES, SHADOW_RES, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	depthCubeMapShader.compileShaders_3(
		Paths::Shaders + "depthCubeMapShader.vert",
		Paths::Shaders + "depthCubeMapShader.geom",
		Paths::Shaders + "depthCubeMapShader.frag"
		);
	depthCubeMapShader.linkShaders();
	
}

void ShadowCubeMap::updateTransforms(const glm::vec3& lightPos)
{
	shadowTransforms.clear();

	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));
}


void ShadowCubeMap::renderDepthCubeMap(
	const std::vector<Entity> entities,
	const ComponentStorage<TransformComponent>& trs,
	const ComponentStorage<MeshFilter>& mfs,
	const ComponentStorage<AnimationComponent>& anims,
	const Entity& mainPointLight,
	AssetManager& assetManager
)
{
	if (mainPointLight == NullEntity || !trs.Has(mainPointLight) || prevLight != mainPointLight) {
		prevLight = mainPointLight;

		// clear previous frame shadow
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return;
	}

	auto& lightTf = trs.Get(mainPointLight);


	depthCubeMapShader.use();
	glm::vec3& lightPos = lightTf.GetWorldPosition();

	// Upload shadow matrices
	updateTransforms(lightPos);

	const auto& matrices = shadowTransforms;
	for (int i = 0; i < 6; ++i) {
		depthCubeMapShader.setMat4(
			"shadowMatrices[" + std::to_string(i) + "]",
			matrices[i]
		);
	}

	depthCubeMapShader.setVec3("lightPos", lightPos);
	depthCubeMapShader.setFloat("farPlane", farPlane);

	glViewport(0, 0, SHADOW_RES, SHADOW_RES);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (auto& e : entities)
	{
		if (!trs.Has(e) || !mfs.Has(e) || e == mainPointLight) continue;;

		auto& tr = trs.Get(e);
		depthCubeMapShader.setMat4("model", tr.worldMatrix);
		auto& mf = mfs.Get(e);
		auto* mesh = assetManager.GetSubmesh(mf.meshID);

		if (!mesh) continue;


		bool hasSkeleton = false;
		const AnimationComponent* anim = nullptr;
		std::vector<glm::mat4> allBones;
		std::vector<glm::mat4> bones;
		std::vector<int> pallete;

		if (anims.Has(mf.rootParent))
		{
			anim =
				&anims.Get(mf.rootParent);

			if (!anim->finalBoneMatrices.empty()
				&& !mesh->bonePalette.empty())
			{
				hasSkeleton = true;
				allBones = anim->finalBoneMatrices;
				pallete = mesh->bonePalette;
			}
		}

		if (hasSkeleton)
		{
			depthCubeMapShader.setBool("useSkeleton", hasSkeleton);

			bones.resize(pallete.size());
			for (size_t i = 0; i < pallete.size(); ++i)
				bones[i] = allBones[pallete[i]];

			for (int i = 0; i < (int)bones.size(); ++i)
				depthCubeMapShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", bones[i]);
		}


		mesh->draw();
		
	}

	depthCubeMapShader.unuse();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
