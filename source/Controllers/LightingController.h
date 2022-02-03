#pragma once
#include "../Shaders/Shader.h"
#include <glm\ext\matrix_float4x4.hpp>
#include <vector>
#include "../Components/MeshRendererComponent.h"

using glm::Matrix4x4;

class LightingController final
{
private:
	LightingController() = delete;

	static unsigned int depthMapFBO;
	static unsigned int SHADOW_WIDTH;
	static unsigned int SHADOW_HEIGHT;
	static unsigned int depthMap;

	static GLint lightSpaceMatrixID;

	static void RecalculateDepthMap();
public:
	static Matrix4x4 lightProjection;
	static Matrix4x4 lightView;
	static Matrix4x4 lightSpaceMatrix;
	static Vector3 lightPos;
	static Vector3 lightRot;

	static Shader* depthShader;
	static void Initialize();
	static void SetShadowMapScale(unsigned int width, unsigned int height);
	static void PrepareDepthMap(std::vector<MeshRendererComponent*> meshesWithShadows);
	static GLuint GetDepthMapID();
};