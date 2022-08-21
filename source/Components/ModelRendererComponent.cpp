#pragma once

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include "ModelRendererComponent.h"
#include "../Shaders/Shader.h"
#include "../Controllers/LightingController.h"

void ModelRendererComponent::SetShader(const Shader* newShader)
{
	currentShader = newShader;
}

void ModelRendererComponent::Render()
{
	if (model == nullptr)
		return;

	currentShader->Use();

	// attach MVP matrix
	glUniformMatrix4fv(
		glGetUniformLocation(currentShader->ID, "MVP"),
		1,
		GL_FALSE,
		&gameObject->transform->GetMVPMatrix()[0][0]);
	currentShader->setMat4("projection", EventSystem::GetMainCamera()->GetProjectionMatrix());
	currentShader->setMat4("view", EventSystem::GetMainCamera()->GetViewMatrix());
	currentShader->setMat4("model", gameObject->transform->GetModelMatrix());
	currentShader->setVec3("lightPos", LightingController::lightPos);
	currentShader->setMat4("lightSpaceMatrix", LightingController::lightSpaceMatrix);
	currentShader->setVec3("viewPos", EventSystem::GetMainCamera()->gameObject->transform->GetPosition());

	// attach light direction vector
	glUniform3fv(
		glGetUniformLocation(currentShader->ID, "lightDirection"),
		1,
		&LightingController::lightPos[0]);

	for (auto& mesh : model->meshes)
		mesh->Render(*currentShader);

	glUseProgram(0);
}

void ModelRendererComponent::RenderDepth(const Shader* depthShader)
{
	if (model == nullptr)
		return;

	depthShader->Use();

	depthShader->setMat4("model", gameObject->transform->GetModelMatrix());
	depthShader->setMat4("lightSpaceMatrix", LightingController::lightSpaceMatrix);

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
