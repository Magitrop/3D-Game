#pragma once
#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include "LightingController.h"
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include "../Shaders/Shader.h"

unsigned int LightingController::depthMapFBO;
unsigned int LightingController::SHADOW_WIDTH;
unsigned int LightingController::SHADOW_HEIGHT;
unsigned int LightingController::depthMap;

const Shader* LightingController::depthShader;
Matrix4x4 LightingController::lightProjection;
Matrix4x4 LightingController::lightView;
Matrix4x4 LightingController::lightSpaceMatrix;
Vector3 LightingController::lightPos;
Vector3 LightingController::lightRot;

GLint LightingController::lightSpaceMatrixID;

void LightingController::RecalculateDepthMap()
{
	float near_plane = 0.1f, far_plane = 100.f;
	//lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, near_plane, far_plane);
	lightProjection = glm::perspective(glm::radians(90.0f), Initializer.GetAspectRatio(), near_plane, far_plane);
	/*lightView =
		glm::rotate(Matrix4x4(1), glm::radians(60.f), Vectors::right) *
		glm::rotate(Matrix4x4(1), glm::radians(lightRot.y), Vectors::up) *
		glm::translate(Matrix4x4(1), -lightPos);*/
	lightView =
		glm::lookAt(
			lightPos,
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f));
	/*for (int x = 0; x < 4; x++, cout << endl)
		for (int y = 0; y < 4; y++)
			cout << lightView[x][y] << " ";
	cout << endl;*/
	lightSpaceMatrix = lightProjection * lightView;
}

void LightingController::Initialize()
{
	SetShadowMapScale(Initializer.windowSize.x * 4, Initializer.windowSize.y * 4);

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
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	lightSpaceMatrixID = glGetUniformLocation(depthShader->ID, "lightSpaceMatrix");
}

void LightingController::SetShadowMapScale(unsigned int width, unsigned int height)
{
	SHADOW_WIDTH = width;
	SHADOW_HEIGHT = height;
	RecalculateDepthMap();
}

void LightingController::PrepareDepthMap(std::vector<ModelRendererComponent*> meshesWithShadows)
{
	RecalculateDepthMap();

	depthShader->Use();
	depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GetDepthMapID());
	for (int i = 0; i < meshesWithShadows.size(); i++)
		meshesWithShadows[i]->RenderDepth(depthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint LightingController::GetDepthMapID()
{
	return depthMap;
}
