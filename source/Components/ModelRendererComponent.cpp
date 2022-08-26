#pragma once

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include "ModelRendererComponent.h"
#include "../Shaders/Shader.h"
#include "../Controllers/LightingController.h"
#include "../Controllers/ResourceManager.h"

void ModelRendererComponent::SetShader(const Shader* newShader)
{
	currentShader = newShader;
}

void ModelRendererComponent::Render()
{
	if (model == nullptr)
		return;

	currentShader->Use();

	currentShader->setMat4("MVP", gameObject->transform->GetMVPMatrix());
	currentShader->setMat4("projection", EventSystem::GetMainCamera()->GetProjectionMatrix());
	currentShader->setMat4("view", EventSystem::GetMainCamera()->GetViewMatrix());
	currentShader->setMat4("model", gameObject->transform->GetModelMatrix());
	currentShader->setVec3("lightPos", LightingController::lightPos);
	currentShader->setMat4("lightSpaceMatrix", LightingController::GetLightSpaceMatrix());
	currentShader->setVec3("viewPos", EventSystem::GetMainCamera()->gameObject->transform->GetPosition());
	currentShader->setVec3("lightDirection", LightingController::lightPos);
	currentShader->SetBool("receiveShadows", receiveShadows);

	for (auto& mesh : model->meshes)
		mesh->Render(*currentShader);

	glUseProgram(0);
}

void ModelRendererComponent::RenderDepth()
{
	if (model == nullptr || !castShadows)
		return;

	LightingController::depthShader->Use();
	LightingController::depthShader->setMat4("model", gameObject->transform->GetModelMatrix());
	LightingController::depthShader->setMat4("lightSpaceMatrix", LightingController::GetLightSpaceMatrix());

	for (auto& mesh : model->meshes)
	{
		glBindVertexArray(mesh->VAO);
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, &mesh->indices[0]);
		glBindVertexArray(0);
	}

	glUseProgram(0);
}

void ModelRendererComponent::SetModel(const Model* newModel)
{
	model = newModel;
}
