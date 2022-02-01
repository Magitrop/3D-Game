#pragma once
#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include "LightingController.h"
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_clip_space.hpp>

unsigned int LightingController::depthMapFBO;
unsigned int LightingController::SHADOW_WIDTH;
unsigned int LightingController::SHADOW_HEIGHT;
unsigned int LightingController::depthMap;

Shader* LightingController::depthShader;
Matrix4x4 LightingController::lightProjection;
Matrix4x4 LightingController::lightView;
Matrix4x4 LightingController::lightSpaceMatrix;

GLint LightingController::lightSpaceMatrixID;

void LightingController::RecalculateDepthMap()
{
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView =
		glm::lookAt(
			glm::vec3(-2.0f, 4.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}

void LightingController::Initialize()
{
	if (glIsFramebuffer(depthMapFBO))
		glDeleteFramebuffers(1, &depthMapFBO);
	glGenFramebuffers(1, &depthMapFBO);

	if (glIsTexture(depthMap))
		glDeleteTextures(1, &depthMap);
	glGenTextures(1, &depthMap);

	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	lightSpaceMatrixID = glGetUniformLocation(depthShader->ID, "lightSpaceMatrix");
	SetShadowMapScale(1024, 1024);
}

void LightingController::SetShadowMapScale(unsigned int width, unsigned int height)
{
	SHADOW_WIDTH = width;
	SHADOW_HEIGHT = height;
	RecalculateDepthMap();
}

void LightingController::PrepareDepthMap(std::vector<MeshRendererComponent*> meshesWithShadows)
{
	depthShader->Use();
	glUniformMatrix4fv(lightSpaceMatrixID, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < meshesWithShadows.size(); i++)
		meshesWithShadows[i]->RenderDepth(depthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint LightingController::GetDepthMapID()
{
	return depthMap;
}
